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
  filtered_model = model$ModelFit %>% filter(ref == plot_ref)
  to_plot = data.frame(Position = c(filtered_model$start, filtered_model$end),
                       Value = rep(filtered_model$fit, 2))
  plt = plt + geom_line(data = to_plot,
                        aes( x = Position,
                             y = Value),
                        lwd = 2)
  # Add points on top
  plt = plt + geom_segment(data=tidy_input, aes(x = start,
                                           xend = end,
                                           y = vals,
                                           yend = vals,
                                           color = batch),
                           lwd = 2)
  plt = plt + ggtitle(plot_ref) + theme_bw() + scale_colour_manual(values=cbPalette)
  
  if (is.null(dir)){
    print(plt)
  }
}