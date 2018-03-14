#########################################
# Tests of actual proeteoformR function #
#########################################
library(dplyr)
library(tidyr)
library(proteoformR)

TurnoverData <- read.csv("./tests/TurnoverSample.csv")

proteoformR(data = TurnoverData,
            ref = Reference,
            start = Start,
            end = End,
            batch = Replicate,
            vals = Norm_Log2_HtoL_TO,
            lambda = 10)
'
SizeExcData <- read.csv("./tests/SizeExcSample.csv")
SizeExcData = cbind(SizeExcData, Replicate = rep("rep1", dim(SizeExcData)[1]))

proteoformR(data = SizeExcData,
            ref = Reference,
            start = Start,
            batch = Replicate,
            vals = LogInequality,
            lambda = 8)


TMData <- read.csv("./tests/TMSamples.csv")

proteoformR(data = TMData,
            ref = "protein_name",
            pos = "start_pos",
            batch = "replicate",
            vals = "meltPoint",
            lambda = 125)'
