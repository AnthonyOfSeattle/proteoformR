proteoformR <- function(data, ref, pos, batch, vals,
                        lambda = 1, method = "PL_L0"){ 
  cleaned_data = data %>%
    select(ref, pos, batch, vals)
  colnames(cleaned_data) = c("ref","pos", "batch" , "vals")
  cleaned_data = cleaned_data %>%
    group_by(ref, batch, pos) %>%
    summarise(med = median(vals)) %>%
    spread(batch, med)
  cleaned_data = cleaned_data[with(cleaned_data, order(ref, pos)),]


  
  model <- list("Input" = cleaned_data)
  print(lambda)
  
  if (method == "PL_L0"){
    model[["BreakPoints"]] = FitBreakPoints(reference = cleaned_data$ref,
                                            pos = cleaned_data$pos,
                                            val = as.matrix(cleaned_data[,-(1:2)]),
                                            lambda)
  }
  
  class(model) = "bpmodel"
  return(model)
}


