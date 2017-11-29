from poloniex import Poloniex
from sklearn.linear_model import LinearRegression
import helper_functions as hf
import matplotlib.pyplot as plt
from sklearn.model_selection import train_test_split

# define the currency pair we will be performing with
currency_pair = ['BTC_ETH']

# retrieve the crypto-currency chart data
chart_data = hf.glueCurrencies(currency_pair, Poloniex())

# create a Poloniex api model according to the given currency pairs
polo_model = hf.create_poloniex_model(chart_data)

# create the needed parameters for the linear regression model and its prediction methods
processed_model, predict, x_test, y_test = hf.poloniex_linear_regression(polo_model, LinearRegression()).values()

# draw the linear regression prediction line and print the next estimated price for the mentioned unix time period
hf.draw_prediction(predict, processed_model, x_test, y_test)
