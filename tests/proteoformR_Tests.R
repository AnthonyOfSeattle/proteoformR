#########################################
# Tests of actual proeteoformR function #
#########################################
library(proteoformR)

TurnoverData <- read.csv("./tests/TurnoverSample.csv")

fit <- proteoformR(data = TurnoverData,
            ref = Reference,
            start = Start,
            end = End,
            batch = Replicate,
            vals = Norm_Log2_HtoL_TO,
            lambda = 10)
fit
plot(fit, "YLR167W")

SizeExcData <- read.csv("./tests/SizeExcSample.csv")
SizeExcData = cbind(SizeExcData, Replicate = rep("rep1", dim(SizeExcData)[1]))

fit2 <- proteoformR(data = SizeExcData,
            ref = Reference,
            start = Start,
            end = End,
            batch = Replicate,
            vals = LogInequality,
            lambda = 10)
fit2
plot(fit2)

TMData <- read.csv("./tests/TMSamples.csv")

fit3 <- proteoformR(data = TMData,
            ref = protein_name,
            start = start_pos,
            end = end_pos,
            batch = replicate,
            vals = meltPoint,
            lambda = 8.5)
fit3
plot(fit3, "YCR030C")

