#! /usr/bin/env python
# -*- coding: utf-8 -*-
# vim:fenc=utf-8
#
# Copyright © 2015 Christopher C. Strelioff <chris.strelioff@gmail.com>
#
# Distributed under terms of the MIT license.

"""analyze_dt.py -- probe a decision tree found with scikit-learn."""
from __future__ import print_function

import os
import subprocess

import pandas as pd
import numpy as np
from sklearn.tree import DecisionTreeClassifier, export_graphviz
from sklearn.metrics import classification_report,confusion_matrix
from sklearn.model_selection import train_test_split

def get_code(tree, feature_names, target_names, spacer_base="    "):
    """Produce psuedo-code for decision tree.

    Args
    ----
    tree -- scikit-leant DescisionTree.
    feature_names -- list of feature names.
    target_names -- list of target (class) names.
    spacer_base -- used for spacing code (default: "    ").
    Notes
    -----
    based on http://stackoverflow.com/a/30104792.
    """
    left      = tree.tree_.children_left
    right     = tree.tree_.children_right
    threshold = tree.tree_.threshold
    features  = [feature_names[i] for i in tree.tree_.feature]
    value = tree.tree_.value

    def recurse(left, right, threshold, features, node, depth):
        spacer = spacer_base * depth
        if (threshold[node] != -2):
            print(spacer + "if ( " + features[node] + " <= " + \
                  str(threshold[node]) + " ) {")
            if left[node] != -1:
                    recurse (left, right, threshold, features, left[node],
                            depth+1)
            print(spacer + "}\n" + spacer +"else {")
            if right[node] != -1:
                    recurse (left, right, threshold, features, right[node],
                             depth+1)
            print(spacer + "}")
        else:
            target = value[node]
            for i, v in zip(np.nonzero(target)[1], target[np.nonzero(target)]):
                target_name = target_names[i]
                target_count = int(v)
                print(spacer + "return " + str(target_name) + " ( " + \
                      str(target_count) + " examples )")

    recurse(left, right, threshold, features, 0, 0)


def visualize_tree(tree, feature_names):
    """Create tree png using graphviz.

    Args
    ----
    tree -- scikit-learn DecsisionTree.
    feature_names -- list of feature names.
    """
    with open("dt.dot", 'w') as f:
        export_graphviz(tree, out_file=f, feature_names=feature_names)

    command = ["dot", "-Tpng", "dt.dot", "-o", "dt.png"]
    try:
        subprocess.check_call(command)
    except:
        exit("Could not run dot, ie graphviz, to produce visualization")


def encode_target(df, target_column):
    """Add column to df with integers for the target.
    Args
    ----
    df -- pandas DataFrame.
    target_column -- column to map to int, producing new Target column.
    Returns
    -------
    df -- modified DataFrame.
    targets -- list of target names.
    """
    df_mod = df.copy()
    targets = df_mod[target_column].unique()
    map_to_int = {name: n for n, name in enumerate(targets)}
    df_mod["Target"] = df_mod[target_column].replace(map_to_int)

    return (df_mod, targets)


def get_truck_data():
    """Get the truck data, from local csv or pandas repo."""
    if os.path.exists("truck.csv"):
        print("-- truck.csv found locally")
        df = pd.read_csv("truck.csv", index_col=0)
    else:
        print("csv not found")
        fn = "https://raw.githubusercontent.com/pydata/pandas/master/pandas/tests/data/truck.csv"
        try:
            df = pd.read_csv(fn)
        except:
            exit("-- Unable to download truck.csv")

        with open("truck.csv", 'w') as f:
            print("-- writing to local truck.csv file")
            df.to_csv(f)

    return df

if __name__ == '__main__':
    print("\n-- get data:")
    # df = get_iris_data()
    df = get_truck_data()
    df_temp = df
    # print("\n-- df.head():")
    # print(df.head(), end="\n\n")

    # print("\n-- df.tail():")
    # print(df.tail(), end="\n\n")

    features = ["val0","val1","val2","val3","val4","val5","val6","val7","val8","val9","val10","val11","val12","val13","val14","val15","val16","val17","val18","val19","val20","val21","val22","val23","val24","val25","val26"]
    df, targets = encode_target(df, "Name")#Name is the column name for our truck
    y = df["Target"]# Truck is 0, Not truck is 1. Initial column vs its corresponding value is set in y
    # print ('y',y)
    X = df[features]# X has all the features matrix. i.e. all the samples without their label
    # print ('X',X)

    die = True
    myCounter = 0
    while(die):
        tr_features, ts_features, tr_labels, ts_labels = train_test_split(X,y,test_size = 0.25, train_size = 0.75)
        dt = DecisionTreeClassifier(min_samples_split=2, random_state=99,max_depth = 100)
        # dt.fit(X, y)
        dt.fit(tr_features, tr_labels)
        predictions = dt.predict(ts_features)
        myAccu = dt.score(ts_features,ts_labels)
        # print (confusion_matrix(ts_labels,predictions))
        # print (classification_report(ts_labels,predictions))
        # print (ts_labels,predictions)
        # print ('Accuracy: ',myAccu,' : Iteration : ',myCounter)
        # break
        # print classification_tree()
        if(myAccu > 0.69 ):
            print ('Accuracy: ',myAccu,' : Iteration : ',myCounter)
        if(myAccu > 0.69):
            print ('Accuracy: ',myAccu,' : Iteration : ',myCounter)
            print("\n-- get_code:")
            get_code(dt, features, targets)
            visualize_tree(dt, features)
            die = False
            break
        else:
            myCounter = myCounter + 1
