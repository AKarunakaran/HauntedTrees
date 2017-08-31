library(isotone)
length = 100
top = length/2
max = length
x = 0:(length-1)
y_asc = c(1: top) * max / top
y_dec = rev(y_asc)
#y = c(y_asc, y_dec)+rnorm(length, mean = 0, sd = length/10) ##response values
y = c(y_asc, y_dec)
w_vec = rep(1,length)              ##unit weights
y_new = gpava(x, y, solver = weighted.median)[[1]]


par(mfrow=c(1,2)) # all plots on one page 
plot(x, y, main = "Before Regression")
plot(x, y_new, main="After Regression")
lines(x, y_new, type = "s")