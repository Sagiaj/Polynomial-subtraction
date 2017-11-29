import numpy as np


class PoloniexData:
    def __init__(self, data):
        self.data_points = []
        self.extract_row_data(data)

    def extract_row_data(self, rows):
        data_points = []
        for row in rows:
            # extract current row data and build the avg price
            row_atts = {'date': row['date'], 'weightedAverage': row['weightedAverage']}
            data_points.append(row_atts)

        self.data_points = data_points

    def get_data_points(self):
        return self.data_points
