bp.fstat <- function(model){
  stripped.input = model$Input %>%
    ungroup() %>%
    select(-(2:3))
  model.resid = stripped.input %>%
    mutate_at(.vars = -1, function(col){col - model$ModelFit$fit}) %>%
    gather("batch", "resid", -1)
  null.resid = stripped.input %>%
    gather("batch", "resid", -1)
  total.resid = model.resid %>%
    mutate(null.resid = null.resid$resid)
  
  breakpoint.numbers = sapply(unique(model$Input$ref),
                              function(r) sum(model$BreakPoints$ref == r))
  peptide.numbers = sapply(unique(model$Input$ref),
                           function(r) sum(model.resid$ref == r))
  f.table = total.resid %>%
    group_by(ref) %>%
    summarise(TSS = sum(null.resid^2, na.rm = TRUE),
              RSS = sum(resid^2, na.rm = TRUE)) %>%
    mutate(between.df = breakpoint.numbers) %>%
    mutate(residual.df = peptide.numbers - breakpoint.numbers - 1) %>%
    mutate(f.stat = TSS/RSS - 1) %>%
    mutate(f.stat = ifelse(f.stat > 0 & between.df > 0,
                           f.stat*(residual.df/between.df),
                           NA)
           ) %>%
    mutate(p.value = ifelse(is.na(f.stat),
                            NA,
                            pf(f.stat,
                               between.df,
                               residual.df,
                               lower.tail = FALSE))
           )
  return(f.table)
}

bp.eval <- function(model, test = "F") {
  # Build effect size statistics
  evaluation = model$ModelFit %>%
    group_by(ref) %>%
    summarize(diff = max(fit) - min(fit))
  
  # Gather p-values
  if (test == "F"){statistics = bp.fstat(model)}
  else (stop(paste(test, "not supported")))

  evaluation = inner_join(evaluation, statistics,
                          by = "ref")
  
  return(evaluation)
}