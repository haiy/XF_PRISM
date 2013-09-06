
Naive Bayes Classifier

                Class
Attribute           0      1
               (0.44) (0.56)
=============================
index0
  0               17.0   49.0
  1               41.0   25.0
  [total]         58.0   74.0

index1
  0               17.0   49.0
  1               41.0   25.0
  [total]         58.0   74.0

index2
  0               17.0   49.0
  1               41.0   25.0
  [total]         58.0   74.0

index3
  0               17.0   49.0
  1               41.0   25.0
  [total]         58.0   74.0

index5
  0               15.0   55.0
  1               43.0   19.0
  [total]         58.0   74.0



Time taken to build model: 0.01 seconds
Time taken to test model on training data: 0.01 seconds

=== Error on training data ===

Correctly Classified Instances         116               90.625  %
Incorrectly Classified Instances        12                9.375  %
Kappa statistic                          0.811 
Mean absolute error                      0.2144
Root mean squared error                  0.2958
Relative absolute error                 43.5544 %
Root relative squared error             59.624  %
Total Number of Instances              128     


=== Confusion Matrix ===

  a  b   <-- classified as
 52  4 |  a = 0
  8 64 |  b = 1



=== Stratified cross-validation ===

Correctly Classified Instances         111               86.7188 %
Incorrectly Classified Instances        17               13.2813 %
Kappa statistic                          0.7296
Mean absolute error                      0.2279
Root mean squared error                  0.3121
Relative absolute error                 46.2651 %
Root relative squared error             62.879  %
Total Number of Instances              128     


=== Confusion Matrix ===

  a  b   <-- classified as
 47  9 |  a = 0
  8 64 |  b = 1

