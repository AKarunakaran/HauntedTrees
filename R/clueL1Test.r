library(clue)
par(mfrow=c(1,2)) # all plots on one page
length = 50
top = length
max = length
x = 0:(length-1)
y_asc = c(1: top) * max / top
y_dec = (c((top+1):length) - length) * max/(top-length)
y = y_asc# + rnorm(length, mean = 0, sd = length/10)
y_vec = c(0, y)
y_mat = matrix(nrow = (length+1), ncol = (length+1))
for(i in 1:(length+1)) {
  for(j in 1:(length+1)) {
    y_mat[i,j] = abs(y_vec[j]-y_vec[i])
  }
}
plot(x, y)
y_vec = as.dist(y_mat)
y_new = l1_fit_ultrametric(y_vec)
y_res = y_new[1][1,]
plot(y_res)

# data("Phonemes")
# d2 = 1-Phonemes
# ## Note that the Phonemes data set has the consonant misclassification
# ## probabilities, i.e., the similarities between the phonemes.
# d <- as.dist(Phonemes)
# 
# ## Find the maximal dominated and miminal dominating ultrametrics by
# ## hclust() with single and complete linkage:
# y1 <- hclust(d, "single")
# y2 <- hclust(d, "complete")
# ## Note that these are quite different:
# cl_dissimilarity(y1, y2, "gamma")
# ## Now find the L2 optimal members of the respective dendrogram
# ## equivalence classes.
# u1 <- l1_fit_ultrametric_target(d, y1)
# u2 <- l1_fit_ultrametric_target(d, y2)
# ## Compute the L2 optimal ultrametric approximation to d.
# u <- l1_fit_ultrametric(d)
# ## And compare ...
# cl_dissimilarity(cl_ensemble(Opt = u, Single = u1, Complete = u2), d)
# ## The solution obtained via complete linkage is quite close:
# cl_agreement(u2, u, "cophenetic")