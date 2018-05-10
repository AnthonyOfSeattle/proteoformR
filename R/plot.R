#' View all or a subset of breakpoint models
#' 
#' \code{plot.bpmodel} builds ggplot2 breakpoint figures for criticism. 
#' The default behavior is to build figures for all references that proteoformR called breakpoints and save them to the working directory. 
#' Uses a color safe pallate for now, which can plot up to 8 samples.
#' 
#' @param model S3 object supplied by running proteoformR
#' @param ref A vector of references to build plots for. The list of references (only.calls == FALSE) or only those with breakpoint calls (only.calls == TRUE) 
#' will be passed unfiltered if ref is set to "All", and will be filtered for only the first if ref is NULL.
#' @param dir Directory to save plot if supplied, but NULL specifies print.
#' @param only.calls Whether to plot only references with breakpoints called if only.calls == TRUE.
#' 
#' @seealso \code{\link{proteoformR}}

plot.bpmodel <- function(model, ref = NULL, dir = NULL, only.calls = FALSE){
  if (only.calls){
    ref_list = unique(model$BreakPoints$ref)
  } else { 
    ref_list = unique(model$Input$ref)
  }
  if (is.null(ref)){
    ref_list = ref_list[1]
  } else if (ref == "All"){
    ref_list = ref_list
  } else{
    ref_list = ref
  }
  for (r in ref_list){
    plt = .build.single.plot(model, plot_ref = r, dir = dir)
    if (is.null(dir)){
      print(plt)
    } else {
      ggsave(filename= paste(dir, "/",
                             r, ".png",
                             sep = ""), dplot = plt)
    }
  }
}

# Global color blind pallate for plotting
cbPalette <- c("#999999", "#E69F00", "#56B4E9", "#009E73", "#F0E442", "#0072B2", "#D55E00", "#CC79A7")

# Create a single plot and determine where to send it
.build.single.plot <- function(model, plot_ref, dir){
  
  plt = ggplot()
  
  # Add model first so it is behind then add points on top
  plt = plt + 
    geom_line(data = model$ModelFit %>% 
                filter(ref == plot_ref) %>%
                mutate(pos = (start+end)/2 ),
              aes( x = pos,
                   y = fit),
              lwd = 2) +
    geom_segment(data= model$Input %>%
                   filter(ref == plot_ref) %>%
                   gather("batch", "vals", 4:dim(.)[2]),
                 aes(x = start,
                     xend = end,
                     y = vals,
                     yend = vals,
                     color = batch),
                 lwd = 2)
  
  # Add default features
  plt = plt + 
    ggtitle(plot_ref) + 
    xlab("Position") +
    ylab("Z-score") +
    theme_bw() + 
    scale_colour_manual("Sample",
                        values=cbPalette) +
    theme(axis.text=element_text(size=14),
          axis.title=element_text(size=16,face="bold"),
          legend.text=element_text(size = 14),
          legend.title=element_text(size = 16))
          
  return(plt)
}