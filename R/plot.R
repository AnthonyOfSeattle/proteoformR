plot.bpmodel.single <- function(model, ref = NULL, dir = NULL){
  if (is.null(ref)){
    plot_ref = unique(model$Input$ref)[1]
  } else {
    plot_ref = ref
  }
  
  tidy_input = model$Input %>%
    filter(ref == plot_ref) %>%
    mutate(peptide = as.factor(1:dim(.)[1])) %>%
    gather("batch", "vals", 4:6)

  cbPalette <- c("#999999", "#E69F00", "#56B4E9", "#009E73", "#F0E442", "#0072B2", "#D55E00", "#CC79A7")
  plt = ggplot(tidy_input, aes(x = start,
                               xend = end,
                               y = vals,
                               yend = vals,
                               color = batch))
  plt = plt + geom_segment(lwd = 2)
  plt = plt + ggtitle(plot_ref) + theme_bw() + scale_colour_manual(values=cbPalette)
  
  if (is.null(dir)){
    print(plt)
  }
}