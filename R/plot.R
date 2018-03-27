plot.bpmodel.single <- function(model, ref = NULL, dir = NULL){
  if (is.null(ref)){
    plot_ref = unique(model$Input$ref)[1]
  } else {
    plot_ref = ref
  }
  
  tidy_input = model$Input %>%
    filter(ref == plot_ref) %>%
    gather("batch", "vals", 4:dim(.)[2])

  cbPalette <- c("#999999", "#E69F00", "#56B4E9", "#009E73", "#F0E442", "#0072B2", "#D55E00", "#CC79A7")
  plt = ggplot()
  
  # Add model first so it is behind
  filtered_model = model$ModelFit %>% 
    filter(ref == plot_ref) %>%
    mutate(pos = (start+end)/2 )
  plt = plt + geom_line(data = filtered_model,
                        aes( x = pos,
                             y = fit),
                        lwd = 2)

  # Add points on top
  plt = plt + geom_segment(data=tidy_input, aes(x = start,
                                           xend = end,
                                           y = vals,
                                           yend = vals,
                                           color = batch),
                           lwd = 2)
  
  plt = plt + ggtitle(plot_ref) + theme_bw() + scale_colour_manual("Sample",
                                                                   values=cbPalette)
  plt = plt + xlab("Position") + ylab("Value")
  plt = plt + theme(axis.text=element_text(size=14),
                    axis.title=element_text(size=16,face="bold"),
                    legend.text=element_text(size = 14),
                    legend.title=element_text(size = 16))
  
  if (is.null(dir)){
    print(plt)
  } else {
    ggsave(filename= paste(dir, "/",
                           plot_ref, ".png",
                           sep = ""),
           plot = plt)
  }
}

plot.bpmodel <- function(model, ref = "All", dir = getwd(), only.calls = TRUE){
  if (ref == "All") {
    if(only.calls){
      ref = unique(model$BreakPoints$ref)
    } else { 
        ref = unique(model$Input$ref)
        }
  }
  for (r in ref){
    plot.bpmodel.single(model, ref = r, dir = dir)
  }
}