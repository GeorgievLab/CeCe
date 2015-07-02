
# Read data
data = read.csv(file = "gfp.csv", sep = ";", header = TRUE)
data$id = factor(data$id)

# Calculate mean values
dataMean = aggregate(data$gfp, by = list(data$totalTime), FUN = mean)
colnames(dataMean) = c("totalTime", "gfp")
dataMean$id = factor(0)

require(ggplot2)

# Plot data
ggplot(data, aes(x = totalTime, y = gfp, colour = id)) + 
	geom_line() + 
	geom_line(data = dataMean, aes(x = totalTime, y = gfp), colour = "black") +
	xlab("Time [s]") + ylab("GFP molecules")

ggsave(filename = "gfp.png")

