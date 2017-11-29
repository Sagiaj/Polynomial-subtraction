import numpy as np
import helper_classes as hc
import time
from sklearn.model_selection import train_test_split
from poloniex import Poloniex
from sklearn.linear_model import LinearRegression
import matplotlib.pyplot as plt

def create_poloniex_model(polo_data):
    polo_model = hc.PoloniexData(polo_data)

    return polo_model


def glueCurrencies(currencies, polo):
    currencies_data = []
    for currency in currencies:
        for data in polo.returnChartData(currency, period=300):
            currencies_data.append(data)

    return currencies_data


def poloniex_linear_regression(polo_model, lr_model):
    polo = Poloniex()
    data_points = polo_model.get_data_points()
    x = np.asarray([point['date'] for point in data_points]).astype(np.float)
    y = np.asarray([point['weightedAverage'] for point in data_points]).astype(np.float)
    x_train, x_test, y_train, y_test = train_test_split(x, y, test_size=0.25, random_state=1)
    polo_class = hc.PoloniexData(polo.returnChartData('BTC_ETH', 300, time.time() - 86400, time.time() + 300))
    polo_x_test = np.asarray([point['date'] for point in polo_class.get_data_points()]).astype(np.float)\
        .reshape((-1, 1))
    np.append(polo_x_test, np.add(time.time(), 1800))
    polo_y_test = np.asarray([point['weightedAverage'] for point in polo_class.get_data_points()]).astype(np.float)\
        .reshape((-1, 1))
    x_train = x_train.reshape((-1, 1))
    y_train = y_train.reshape((-1, 1))
    lr_model.fit(x_train, y_train)
    predict = lr_model.predict(polo_x_test)

    return {'model': lr_model, 'predict': predict,
            'x_test': polo_x_test, 'y_test': polo_y_test}


def draw_prediction(prediction, lr_model, x_test, y_test):
    plt.scatter(x_test, y_test, color='black')
    plt.plot(x_test, prediction, color='blue', linewidth=3)

    plt.xticks(())
    plt.yticks(())

    plt.show()
