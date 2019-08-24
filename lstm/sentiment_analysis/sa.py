import pandas as pd

# Load data from csv files
train = pd.read_csv("train.csv", encoding = "ISO-8859-1")
test = pd.read_csv("test.csv", encoding = "ISO-8859-1")

# Get train data and labels
train_data = train.iloc[:, 5:6].values
train_labels = train.iloc[:, 0:1].values

# get test data and labels
test_data = test.iloc[:, 5:6].values
test_labels = test.iloc[:, 0:1].values

# Text preprocessing
print(train_data)
print(test_data)

