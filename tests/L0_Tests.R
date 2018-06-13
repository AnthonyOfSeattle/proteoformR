########################################
# Direct tests of L0 penalty detectors #
########################################
library(dplyr)
library(tidyr)
library(proteoformR)

print("Testing L0 type detectors of turnover data:")

################
# Read in data #
################

TurnoverData <- read.csv("./tests/TurnoverSample.csv")
CleanedData <- TurnoverData %>%
  filter(Reference == "YLR167W") %>%
  select(Reference, Replicate, Start, Norm_Log2_HtoL_TO) %>%
  mutate(Norm_Log2_HtoL_TO = scale(Norm_Log2_HtoL_TO)) %>%
  group_by(Reference, Replicate, Start) %>%
  summarise(med = median(Norm_Log2_HtoL_TO)) %>%
  spread(Replicate, med)
CleanedData <- CleanedData[with(CleanedData, order(Reference, Start)),]

fit <- DetectBreakpoints(val = as.matrix(CleanedData[,-(1:2)]),
                         lambda = 8)
fit

TMData <- read.csv("./tests/TMSamples.csv")
CleanedData <- TMData %>%
  filter(protein_name == "YCR030C") %>%
  select(protein_name, replicate, start_pos, meltPoint) %>%
  group_by(protein_name) %>%
  mutate(meltPoint = scale(meltPoint)) %>%
  group_by(protein_name, replicate, start_pos) %>%
  summarise(med = median(meltPoint)) %>%
  spread("replicate", "med")
CleanedData <- CleanedData[with(CleanedData, order(protein_name, start_pos)),]

fit <- DetectBreakpoints(val = as.matrix(CleanedData[,-(1:2)]),
                         lambda = 8)
fit
