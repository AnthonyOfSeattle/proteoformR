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
#' @return An S3 object with class of type, "bpmodel", which contains the manipulated input, 
#' a dataframe of breakpoints, and a predicted best model. 

proteoformR <- function(data, vals, ref = NULL, start = NULL, end = NULL, batch = NULL,
                        lambda = 10){
  
  # In order to call breakpoints we really only need sequential values
  # so the only required arguments are: data, vals
  if (missing(data) | missing(vals)){
    stop("Please supply both data and a reference to the values column")
  }
  
  # Arguments don't need to be supplied as string "arg"
  # can be supplied: arg
  supplied_args <- c("vals" = as.character(substitute(vals)),
                     "ref" = as.character(substitute(ref)),
                     "start" = as.character(substitute(start)),
                     "end" = as.character(substitute(end)),
                     "batch" = as.character(substitute(batch)))
  
  # Builds a sub data frame with columns from supplied args
  # any missing values from supplied args are inputed with
  # default values.
  parsed_data = data %>% 
    .parse.input(supplied_args)
  
  # Duplicated data which has the same reference, positioning, and batch
  # is averaged to avoid confusion later
  averaged_data = parsed_data %>%
    group_by(ref, start, end, batch) %>%
    summarise(vals = mean(vals, na.rm = TRUE))
  
  # Data needs to be scaled to make sure penalty terms
  # can be applied uniformly
  scaled_data = averaged_data %>%
    group_by(ref, batch) %>%
    mutate(vals = scale(vals))
  
  # The breakpoint detection treats each batch as seperate,
  # and it is much easier to work with if we input data where
  # each column is a batch
  spread_data = scaled_data %>%
    spread(batch, vals) %>%
    arrange(ref, start, end) %>%
    filter( sum(!(is.na(.[,-(1:3)]))) > 0 )
    
  
  model <- list("Input" = spread_data)
  
  startpoint = 0
  breakpoints = c()
  fit = c()
  for (r in unique(spread_data$ref)){
    data.subset = spread_data %>% 
      filter(ref == r)
    breakpoint.subset = DetectBreakpoints(values = as.matrix(data.subset[,-(1:3)]),
                                          lambda)
    fit = c(fit, FitModel(values = as.matrix(data.subset[,-(1:3)]),
                          breakpoints = breakpoint.subset))

    breakpoints = c(breakpoints, breakpoint.subset + startpoint)
    startpoint = startpoint + dim(data.subset)[1]
  }
  model[["BreakPoints"]] = spread_data[breakpoints, colnames(spread_data) %in% names(supplied_args)]
  model[["ModelFit"]] =  cbind(spread_data[,1:3], fit = fit)
  
  class(model) = "bpmodel"
  return(model)
}

# Utility function to tranform an input to proteoformr into a usable dataframe
.parse.input <- function(.input, args){
  parsed_data = data.frame(vals = .input[[ args["vals"] ]]) %>%
    .add.reference(.input, args) %>%
    .add.positions(.input, args) %>%
    .add.batch(.input, args) 
  return(parsed_data)
}

# Add a reference column from data if present, add default if not
.add.reference <- function(.data, input, args){
  if ("ref" %in% names(args)){
    new_data = .data %>% mutate(ref = input[[args["ref"]]])
  } else {
    new_data = .data %>% mutate(ref = rep("Data", dim(input)[1]))
  }
  return(new_data)
}

# Add sepperate start and end if available, otherwise, treat as point measures
.add.positions <- function(.data, input, args){
  if ("start" %in% names(args) &
      "end" %in% names(args)){
    new_data = .data %>% 
      mutate(start = input[[args["start"]]]) %>%
      mutate(end = input[[args["end"]]])
  } else if ("start" %in% names(args)){
    new_data = .data %>% 
      mutate(start = input[[args["start"]]]) %>%
      mutate(end = input[[args["start"]]])
  } else if ("end" %in% names(args)){
    new_data = .data %>% 
      mutate(start = input[[args["end"]]]) %>%
      mutate(end = input[[args["end"]]])
  } else {
    new_data = .data %>% 
      mutate(start = 1:dim(input)[1]) %>%
      mutate(end = 1:dim(input)[1])
  }
  return(new_data)
}

# Add a batch column
.add.batch <- function(.data, input, args){
  if ("batch" %in% names(args)){
    new_data = .data %>% mutate(batch = input[[args["batch"]]])
  } else {
    new_data = .data %>% mutate(batch = rep("Replicate.1", dim(input)[1]))
  }
  return(new_data)
}