library(PreUniRegL1)
length.of.sizes= 20
num.rep.data= 20
y_time=numeric(length.of.sizes)
y_std =numeric(length.of.sizes)
x_size = 100000*(1:length.of.sizes)
for(i in 1:length.of.sizes){
  length = x_size[i]
  times=numeric(num.rep.data)
  top = length
  max = length
  y_asc = c(1: top) * max / top
  y_dec = rev(y_asc)
  y_perf = y_asc
  w_vec = numeric(length) + 1
  
  for(a in 1:num.rep.data){
    #y_vec = y_perf + rnorm(x_size[i], mean = 0, sd = sqrt(x_size[i]))
    y_vec = runif(length, 1, length)
    #error = rep(0, length)
    ptm <- proc.time()
    #y_new = prefixIsoRegL1(y_vec, w_vec, error)
    y_new = unimodalRegL1(y_vec, w_vec)
    temp = proc.time()-ptm
    times[a] = temp[3]
  }
  y_time[i]=mean(times)
  y_std[i] = sd(times)
}
plot(x_size, y_time, pch = 19, main="Time Analysis")
arrows(x_size, y_time-y_std, x_size, y_time+y_std, length=0.05, angle=90, code=3)
abline(lm(y_time~x_size), col="red")
lm(y_time~x_size)
cor(x_size, y_time)