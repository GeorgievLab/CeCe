#options(echo = TRUE)
args <- commandArgs(trailingOnly = TRUE)

if (length(args) == 0) {
  "Missing argument"
  q();
}

svg(width = 15, height = 7)

# Read CSV data
csv_data = read.csv(file = args[1], head = FALSE, sep = ";")
colnames(csv_data) = c("name", "step", "time")
data = split(csv_data, csv_data$name)

first = TRUE
color = 1
max_time = 0

# Foreach types
for (name in names(data)) 
{
  values = data[[name]]
  
  # Calculate maximum time
  max_time = max(max_time, max(values$time, na.rm = TRUE))
}

for (name in names(data)) 
{
  values = data[[name]]
  
  if (first) {
    plot(values$step, values$time, 
         ylim = c(0, max_time),
         main = "Simulator time measurement", 
         xlab = "Step", 
         ylab = expression(paste("Time [", mu, "s]")), 
         col = color,
         type = "l")
  } else {
    points(values$step, values$time, 
           col = color,
           type = "l")
  }
  
  first = FALSE
  color = color + 1
}

legend("top", names(data),
       ncol = 3,
       lwd = rep(2.5, length(data)),
       col = seq(1, length(data))
)

