#' Build a breakpoint model for each reference in a tidy format dataframe. 
#' 
#' \code{proteoformR} uses a Rcpp dynamic programming backend to solve an L0 penalized changepoint model for each protein. 
#' For the best performance, each sample should be treated as a batch, but the only required arguments are data and vals; 
#' all other arguments are suggestions and will only be used if supplied. This function performs sorting and sclaing internally,
#' so don't worry about preprocessing data.
#' 
#' @param data A dataframe in tidy format containing at least one column of continuous values
#' @param vals Column name for values within data
#' @param ref Column name for strings which partition values into protein references
#' @param start Start position for each peptide
#' @param end Ending position for each peptide
#' @param batch Batching information. In most cases this will be the sample number for each measurement.
#' @param lambda Penalty term to apply to each L0 optimization.
#' @param method Deprecated, will likely be reformatted later
#' @return An S3 object with class of type, "bpmodel", which contains the manipulated input, 
#' a dataframe of breakpoints, and a predicted best model. 

proteoformR <- function(data, vals, ref = NULL, start = NULL, end = NULL, batch = NULL,
                        lambda = 10, method = "PL_L0"){
  
  # Add supplied columns
  supplied_args <- c("vals" = as.character(substitute(vals)),
                     "ref" = as.character(substitute(ref)),
                     "start" = as.character(substitute(start)),
                     "end" = as.character(substitute(end)),
                     "batch" = as.character(substitute(batch)))
  cleaned_data = data.frame(vals = data[[ supplied_args["vals"] 
                                          ]])
  # Add reference info
  if ("ref" %in% names(supplied_args)){
    cleaned_data["ref"] = data[[ supplied_args["ref"]
                                 ]]
  } else {
    cleaned_data["ref"] = rep(deparse(substitute(data)), dim(data)[1])
  }
  
  # Add batch info
  if ("batch" %in% names(supplied_args)){
    cleaned_data["batch"] = data[[ supplied_args["batch"]
                                 ]]
  } else {
    cleaned_data["batch"] = rep(deparse("rep1"), dim(data)[1])
  }
  
  # Add start/stop info
  if ("start" %in% names(supplied_args) &
      "end" %in% names(supplied_args)){
    cleaned_data["start"] = data[[supplied_args["start"]
                                  ]]
    cleaned_data["end"] = data[[ supplied_args["end"]
                                     ]]
  } else if ("start" %in% names(supplied_args)){
    cleaned_data["start"] = data[[supplied_args["start"]
                                  ]]
    cleaned_data["end"] = data[[ supplied_args["start"]
                                 ]]
  } else if ("end" %in% names(supplied_args)){
    cleaned_data["start"] = data[[supplied_args["end"]
                                  ]]
    cleaned_data["end"] = data[[ supplied_args["end"]
                                 ]]
  } else {
    cleaned_data["start"] = 1:dim(data)[1]
    cleaned_data["end"] = 1:dim(data)[1]
  }
  # Do final data cleaning
  cleaned_data = cleaned_data %>% 
    group_by(ref, start, end, batch) %>%
    summarise(vals = mean(vals, na.rm = TRUE)) %>%
    group_by(ref) %>%
    mutate(vals = scale(vals)) %>%
    spread(batch, vals) %>%
    arrange(ref, start, end) %>%
    filter( sum(!(is.na(.[,-(1:3)]))) > 0 )


  model <- list("Input" = cleaned_data)
  
  if (method == "PL_L0"){
    startpoint = 0
    breakpoints = c()
    for (r in unique(cleaned_data$ref)){
      data.subset = cleaned_data %>% 
        filter(ref == r)
      breakpoint.subset = FitBreakPoints(reference = data.subset$ref,
                                         val = as.matrix(data.subset[,-(1:3)]),
                                         lambda)
      breakpoints = c(breakpoints, breakpoint.subset + startpoint) 
      startpoint = startpoint + dim(data.subset)[1]
    }
    fit = BuildModel(reference = cleaned_data$ref,
                     val = as.matrix(cleaned_data[,-(1:3)]),
                     breakpoints = breakpoints)
  }
  model[["BreakPoints"]] = cleaned_data[breakpoints, colnames(cleaned_data) %in% names(supplied_args)]
  model[["ModelFit"]] =  cbind(cleaned_data[,1:3], fit = fit)
  
  class(model) = "bpmodel"
  return(model)
}


