
n = 1000
x = rnorm(n)
y = rnorm(n)
p = 2
write.csv(x, "x1.csv", row.names = FALSE)
write.csv(y, "y1.csv", row.names = FALSE)

out = lsfit(x, y, intercept = TRUE)

beta = out$coef
res = out$residual
sigma = sum(res^2) / (n - p)
write.csv(beta, "sol1-0.csv", row.names = FALSE);
write.csv(sigma, "sol1-1.csv", row.names = FALSE);


n = 1000
p = 11
X = matrix(rnorm(n), nrow = 100, ncol = 10, byrow = TRUE)
y = rnorm(100)
write.csv(X, "x2.csv", row.names = FALSE)
write.csv(y, "y2.csv", row.names = FALSE)


out = lsfit(X, y, intercept = TRUE)

beta = out$coef
res = out$residual
sigma = sum(res^2) / (n - p)
write.csv(beta, "sol2-0.csv", row.names = FALSE);
write.csv(sigma, "sol2-1.csv", row.names = FALSE);

XX = matrix(NA, nrow = 2, ncol = 2)
YY = XX
XX[1,1] = 0.1; XX[1,2] = 0.4; XX[2,1] = 0.8; XX[2,2]= 0.7;
YY[1,1] = 0.1; YY[1,2] = 0.4; YY[2,1] = 0.8; YY[2,2] = 0.7;


A = matrix(NA, nrow = 3, ncol = 3)
A[0,0] = 11.2836; A[0,1] = 5.81035; A[0,2] = 2.99262
A[1,0] = 5.81035; A[1,1] = 2.99262; A[1,2] = 1.54169
A[2,0] = 2.99262; A[2,1] = 1.54169; A[2,2]



