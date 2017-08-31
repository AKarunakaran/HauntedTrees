library(clue)
par(mfrow=c(1,1))
length.of.sizes= 8
num.rep.data= 20
y_time=numeric(length.of.sizes)
y_std =numeric(length.of.sizes)
x_size = 10*(1:length.of.sizes)
for(i in 1:length.of.sizes){
  length = x_size[i]
  times=numeric(num.rep.data)
  max = length
  x_vec = 1:length
  y_asc = 1:length
  y_perf = y_asc
  
  for(a in 1:num.rep.data){
    y_vec = y_perf + rnorm(length, mean = 0, sd = sqrt(length))
    y_vec = c(0, y_vec)
    y_mat = matrix(nrow = (length+1), ncol = (length+1))
    for(j in 1:(length+1)) {
      for(k in 1:(length+1)) {
        y_mat[j,k] = abs(y_vec[k]-y_vec[j])
      }
    }
    y_vec = as.dist(y_mat)
    ptm <- proc.time()
    y_new = l1_fit_ultrametric(y_vec)
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