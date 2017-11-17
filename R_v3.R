# Working directory 
# rm(list = ls())
setwd("/Users/kangbowei/Desktop/Study/MSMF courses/fall_2016/628_qrm/qrm_final project")

# Load libraries
library(PerformanceAnalytics)
library(quantmod)
library(xts)
library(timeDate)
library(rugarch)

# periods
start_list = c("2001-01-01","2001-01-01","2005-01-01","2009-01-01","2013-01-01")
end_list = c("2016-12-30","2004-12-31","2008-12-31","2012-12-31","2016-12-30")
start_date <- start_list[4]
end_date <- end_list[4]
start_yr <- strsplit(start_date,"-")[[1]][1]
end_yr <- strsplit(end_date,"-")[[1]][1]
subtitle <- paste(start_yr, "-", end_yr)

# load data
getSymbols("SPY", src='yahoo', from=start_date, to=end_date)
AdjClosePrices <- SPY[,"SPY.Adjusted"]
ndays <- length(AdjClosePrices); print(ndays)
plot(AdjClosePrices, main=paste("SPY index @ ",subtitle))

# returns
ret <- CalculateReturns(AdjClosePrices, method="log")
ret_abs <- abs(ret)
ret_square <- ret * ret

# get acf
acf_ret = acf(ret, type="correlation", na.action=na.pass, plot=FALSE)
acf_ret_abs = acf(ret_abs, type="correlation", na.action=na.pass, plot=FALSE)
acf_ret_square = acf(ret_square, type="correlation", na.action=na.pass, plot=FALSE)
acf_ret$acf[2:6]
acf_ret$acf[2:6]*sqrt(ndays)

# Ljung-Box Tests
h = 24
box_ret <- Box.test(ret, lag=h, type="Ljung-Box")
box_ret_abs <- Box.test(ret_abs, lag=h, type="Ljung-Box")
box_ret_square <- Box.test(ret_square, lag=h, type="Ljung-Box")
print(c(box_ret[[1]], box_ret_abs[[1]], box_ret_square[[1]]))
print(c(box_ret[[3]], box_ret_abs[[3]], box_ret_square[[3]]))
print(qchisq(0.99, h))
print(1/sqrt(ndays))

# plot ret and acf
op <- par(mfrow = c(1, 2))
plot(ret, main=paste("Returns @ ",subtitle))
plot(acf_ret, main=paste("Returns ACF @ ",subtitle))
par(op)
op <- par(mfrow = c(1, 2))
plot(ret_abs, main=paste("Abs Returns @ ",subtitle))
plot(acf_ret_abs, main=paste("Abs Returns ACF @ ",subtitle))
par(op)
op <- par(mfrow = c(1, 2))
plot(ret_square, main=paste("Square Returns @ ",subtitle))
plot(acf_ret_square, main=paste("Square Returns ACF @ ",subtitle))
par(op)

# GARCH
# need to select model specification for each period
sp <- ugarchspec(mean.model=list(armaOrder=c(0,0), include.mean=TRUE),
      variance.model=list(model="eGARCH", garchOrder=c(2,1)), distribution.model="norm")
garch_fit <- ugarchfit(data=ret[!is.na(ret)], spec=sp)
# garch_fit
# garch_pred <- ugarchforecast(garch_fit, n.ahead=1, out.sample = 100)

stan_res_garch <- as.vector(residuals(garch_fit, standardize=TRUE))
acf_res_garch = acf(stan_res_garch, type="correlation", na.action=na.pass, plot=FALSE)
acf_res_garch_abs = acf(abs(stan_res_garch), type="correlation", na.action=na.pass, plot=FALSE)
acf_res_garch_square = acf((stan_res_garch)^2, type="correlation", na.action=na.pass, plot=FALSE)
h = 6
box_ret <- Box.test(stan_res_garch, lag=h, type="Ljung-Box")
box_ret_abs <- Box.test(abs(stan_res_garch), lag=h, type="Ljung-Box")
box_ret_square <- Box.test(stan_res_garch^2, lag=h, type="Ljung-Box")
print(c(box_ret[[1]], box_ret_abs[[1]], box_ret_square[[1]]))
print(c(box_ret[[3]], box_ret_abs[[3]], box_ret_square[[3]]))

# plot acf
op <- par(mfrow = c(1, 3))
plot(acf_res_garch, main=paste("standard residuals ACF @ ",subtitle))
plot(acf_res_garch_abs, main=paste("abs standard residuals ACF @ ",subtitle))
plot(acf_res_garch_square, main=paste("squared standard residuals ACF @ ",subtitle))
par(op)

#garch_fit@fit$se.coef
#coef(garch_fit)
#unconditional variance: omega/(1-alpha-beta)
#uncvariance(garch_fit)
#sigma(garch_fit)

# volatility predicting
# garch
vol_pred_garch <- sigma(garch_fit)
colnames(vol_pred_garch) <- "predicted volatility by garch"

# moving average
# Initial value for variance
S <- 0 
nwindow = 60
TS <- as.numeric(ret_square[2:length(ret_square)])
SMA <- NULL
for (i in 1:length(TS)){    
  if (i <= nwindow) { S <- (S * (i-1)+ TS[i])/i }
  if (i > nwindow) { S <- S + (TS[i]/nwindow)-(TS[i-nwindow]/nwindow) }
  SMA <- c(SMA, sqrt(S))
}
vol_pred_sma <- as.xts(SMA, order.by = index(ret[2:ndays]))
colnames(vol_pred_sma) <- "predicted volatility by moving average"

# plot vol predict
col <- c("grey","red","blue")
lty <- c(1,5,1)
lwd <- c(1,1.5,1)
basket_vol <- cbind(ret_abs[2:length(ret)] ,vol_pred_sma, vol_pred_garch)
plot(as.zoo(basket_vol),screens=1,col=col, lty=lty, lwd=lwd)
legend("topright", legend =c("abs_return","sma","garch"), lty=lty, lwd=lwd, col=col)

# VaR
confid <- 0.975
ret_var <- ret[2:length(ret)]
VaR_pred_sma <- qnorm(confid) * vol_pred_sma
VaR_pred_garch <- qnorm(confid) * vol_pred_garch

# VaR nonparametric
high <- NULL
low <- NULL
for (i in 1:length(ret_var)){
  arr_tmp <- as.numeric(ret_var[1:i])
  ind_high <- ceiling(i * confid)
  ind_low <- ceiling(i* (1-confid))
  high <- c(high, sort(arr_tmp)[ind_high])
  low <- c(low, sort(arr_tmp)[ind_low])
}
VaR_pred_np_high <- as.xts(high, order.by = index(ret[2:ndays]))
VaR_pred_np_low <- as.xts(low, order.by = index(ret[2:ndays]))
colnames(VaR_pred_np_high) <- "Nonparametric VaR"
colnames(VaR_pred_np_low) <- "Nonparametric VaR"

# plot var
basket_var <- cbind(ret[2:length(ret)] ,
                    VaR_pred_sma, -VaR_pred_sma, VaR_pred_garch, -VaR_pred_garch,
                    VaR_pred_np_high, VaR_pred_np_low)
col <- c("grey","red","red","blue","blue","green4","green4")
#col <-  rainbow(ncol(basket_var))
lty <- c(1,1,1,1,1,2,2)
lwd <- c(1,1.5,1.5,1,1,1.5,1.5)
plot(as.zoo(basket_var),screens=1,col=col, lty=lty, lwd=lwd)
legend("topleft", legend =c("returns","VaR_sma","VaR_garch","VaR_np"), 
       lty=c(1,1,1,2), lwd=c(1,1.5,1,1.5), col=c("grey","red","blue","green4"))

# VaR violation
vio_sma <- sum(ret_var > VaR_pred_sma | ret_var < -VaR_pred_sma)
vio_garch <- sum(ret_var > VaR_pred_garch | ret_var < -VaR_pred_garch)
vio_np <- sum(ret_var > VaR_pred_np_high | ret_var < VaR_pred_np_low)
vio_rate_sma <- vio_sma / length(ret_var)
vio_rate_garch <- vio_garch / length(ret_var)
vio_rate_np <- vio_np / length(ret_var)
vio_rate_sma
vio_rate_garch
vio_rate_np

# var test
# Kupiec test function
kp_test <- function(x, n, c){
  # x: exceedence
  # n: number of prediction periods
  # c: confidence level
  # adjust from one tail to two tails
  c1 <- 1 - 2*(1-c)
  c2 <- 1 - c1
  lr <- 2 * log(((1 - x/n)/c1) ^ (n-x) * (x/n/c2) ^ x )
  return (lr)
}

# Christoffersen test function
ch_test <- function(d, varl, varh, c){
  # d: original returns matrix
  # n1: traning sample periods
  # n2: prediction periods
  # var5: VaR(5%) array
  # return lr of ch test
  # adjust from one tail to two tails
  c1 <- 1 - 2*(1-c)
  c2 <- 1 - c1
  n <- length(d)
  
  state <- (d > varh | d < varl)
  state1 <- state[1:(n-1)]
  state2 <- state[2:n]
  state3 <- as.numeric(state1) + as.numeric(state2)
  state4 <- as.numeric(state2) - as.numeric(state1)
  v11 <- length(which(state3 == 2))
  v00 <- length(which(state3 == 0))
  v10 <- length(which(state3 == 1 & state4 == -1))
  v01 <- length(which(state3 == 1 & state4 == 1))
  p <- (v01+v11) / (v01+v11+v00+v10)
  p01 <- v01 / (v00+v01)
  p11 <- v11 / (v10+v11)
  
  L0 <- log((1-p)^(v00+v10)*p^(v01+v11))
  L1 <- log((1-p01)^v00*p01^v01*(1-p11)^v10*p11^v11)
  lr <- 2*L1/L0
  return(lr)
}

# Kupiec and Christoffersen test
qchisq(0.95, 1)
kp_test(vio_sma, length(ret_var), confid) 
kp_test(vio_garch, length(ret_var), confid) 
kp_test(vio_np, length(ret_var), confid) 
ch_test(ret_var, -VaR_pred_sma, VaR_pred_sma, 0.975)
ch_test(ret_var, -VaR_pred_garch, VaR_pred_garch, 0.975)
ch_test(ret_var, VaR_pred_np_low, VaR_pred_np_high, 0.975)









