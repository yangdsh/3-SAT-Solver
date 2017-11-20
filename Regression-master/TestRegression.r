
library(Rcpp)
dyn.load("~/documents/workspace/Regression/libreg.so")


testRegression <- function(n, p){
	

	####################################################
	## Test linear regression 
	####################################################	

	x = matrix(rnorm(n), nrow = n, ncol = p)
	y = rnorm(n)

	regType = "Linear"
	seriesType = "STATIC"
	intercept = TRUE
	degree = 1
	polyCol = NULL
	span = 1
	locWeightStr = NULL
	distFunStr = NULL
	robustWeightStr = NULL
	numRobustItr = NULL

	out = .Call('testRegression', x, y, intercept, regType, degree, polyCol, 
		seriesType, span, locWeightStr, distFunStr, robustWeightStr, numRobustItr)	
	r.out = lsfit(x, y, intercept = TRUE)
	r.sigma = sum(r.out$residual^2) / (n - p - as.numeric(intercept))
	test = rep(p + as.numeric(intercept) + 1)
	for(i in 1:(p + as.numeric(intercept))){
		test[i] = round(abs(as.numeric(r.out$coef[i]) - out[["beta"]][[1]][i]), 4)
	}
	test[p +  as.numeric(intercept) + 1] = round(abs(r.sigma - out[["sigma"]]), 4)
	print(c("Linear: ", test))
	
	
	####################################################
	## Test polynomial  regression 
	####################################################	
	regType = "Poly"
	degree = 2
	polyCol = c(1:p)
	locWeightStr = NULL
	distFunStr = NULL
	robustWeightStr = NULL
	numRobustItr = NULL

	out = .Call('testRegression', x, y, intercept, regType, degree, polyCol, 
		seriesType, span, locWeightStr, distFunStr, robustWeightStr, numRobustItr)	
	
	r.x = x;	
	for(i in polyCol){
		r.x = cbind(r.x, poly(x[,i], degree = degree, raw = TRUE)[,-1])
	}
	r.out = lsfit(r.x, y, intercept = TRUE)
	if( degree >= 1){
		r.sigma = sum(r.out$residual^2) / (n - p - NROW(polyCol) * (degree - 1) -  as.numeric(intercept))
	}else{
		r.sigma = sum(r.out$residual^2) / (n - p -  as.numeric(intercept))
	}
	test = rep(p + as.numeric(intercept) + 1)
	for(i in 1:(p + as.numeric(intercept))){
		test[i] = round(abs(as.numeric(r.out$coef[i]) - out[["beta"]][[1]][i]), 4)
	}
	test[p +  as.numeric(intercept) + 1] = round(abs(r.sigma - out[["sigma"]]), 4)
	print(c("Poly: ", test))
	
	
	####################################################
	## Test LOESS regression 
	####################################################	
	regType = "LOESS"
	locWeightStr = "Tricube"
	distFunStr = "Euclidean"
	robustWeightStr = NULL
	numRobustItr = NULL
	out = .Call('testRegression', x, y, intercept, regType, degree, polyCol, 
		seriesType, span, locWeightStr, distFunStr, robustWeightStr, numRobustItr)	
	
	####################################################
	## Test LOESS regression 
	####################################################	
	# regType = "RLOESS"
	# locWeightStr = "Tricube"
	# distFunStr = "Euclidean"
	# robustWeightStr = "Bisquare"
	# numRobustItr = 2
	# out = .Call('testRegression', x, y, intercept, regType, degree, polyCol, 
	#	seriesType, span, locWeightStr, distFunStr, robustWeightStr, numRobustItr)	
	
}
	
