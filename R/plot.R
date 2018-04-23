#' View a single breakpoint model
#' 
#' \code{plot.bpmodel.single} builds single ggplot2 breakpoint figures for criticism. Can be called specifically,
#' but was originally intended to be called from plot.bpmodel. The default behavior, by leaving
#' ref and dir null, is to plot the first reference and print to stdout. Uses a color safe pallate
#' for now, which can plot up to 8 samples.
#' 
#' @param model S3 object supplied by running proteoformR
#' @param ref The protein reference to extract and plot
#' @param dir Directory to save plot if supplied, but NULL specifies print
#' @seealso \code{\link{proteoformR}} and \code{\link{plot.bpmodel}}

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
  plt = plt + xlab("Position") + ylab("Z-score")
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


#' View all or a subset of breakpoint models
#' 
#' \code{plot.bpmodel} builds ggplot2 breakpoint figures for criticism. 
#' The default behavior is to build figures for all references that proteoformR called breakpoints and save them to the working directory. 
#' Uses a color safe pallate for now, which can plot up to 8 samples.
#' 
#' @param model S3 object supplied by running proteoformR
#' @param ref A vector of references to build plots for. If the default value is used, "All",
#' then plots will be generated for references with breakpoint calls (if only.calls == TRUE), or all references in the set (if only.calls == TRUE).
#' @param dir Directory to save plot if supplied, but NULL specifies print. Defaults to the working directory.
#' @param only.calls Whether to plot only references with breakpoints called if only.calls == TRUE
#' 
#' @seealso \code{\link{proteoformR}}

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