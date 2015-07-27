
# Read data
data = read.csv(file = "molecules.csv", sep = ";", header = TRUE)

# Molecule names
molecules = setdiff(colnames(data), c("iteration", "totalTime", "id"))

require(reshape2)
require(ggplot2)

dataMelted = melt(data[c(molecules, "totalTime")], id = c("totalTime"))

ggplot(dataMelted, aes(x = totalTime, y = value, color = variable)) + geom_line() +
  labs(x = "Time [s]", y = "Number of molecules", color = "Molecule name")

ggsave(filename = "molecules.svg")
