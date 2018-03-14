proteoformR <- function(data, vals, ref = NULL, start = NULL, end = NULL, batch = NULL,
                        lambda = 1, method = "PL_L0"){
  
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
    group_by(ref) %>%
    mutate(vals = scale(vals)) %>%
    spread(batch, vals) %>%
    arrange(ref, start, end)

  model <- list("Input" = cleaned_data)
  
  if (method == "PL_L0"){
    breakpoints = FitBreakPoints(reference = cleaned_data$ref,
                                 val = as.matrix(cleaned_data[,-(1:3)]),
                                 lambda)
  }
  model[["BreakPoints"]] = cleaned_data[breakpoints, colnames(cleaned_data) %in% names(supplied_args)]
  
  class(model) = "bpmodel"
  return(model)
}


