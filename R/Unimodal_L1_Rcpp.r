sourceCpp("/Users/amankarunakaran/documents/college/other/Research/Stout/hauntedtrees/r/unimodalRegL1.cpp")

length = 1000
top = length/2
max = length
x_vec = c(0: (length - 1))
y_asc = c(1: top) * max / top
y_dec = c(length: (top + 1)) * max / length
y_vec = c(y_asc, y_dec) + rnorm(length, mean = 0, sd = length/10)
w_vec = rep(1,length)

par(mfrow=c(1,2)) # all plots on one page 
plot(x_vec, y_vec, main = "Before Regression")
y_new = unimodalRegL1(y_vec, w_vec)
plot(x_vec, y_new, main="After Regression")
lines(x_vec, y_new, type = "s")
y_new
y_vec
