
Naive Bayes Classifier

                Class
Attribute           0      1
               (0.44) (0.56)
=============================
index2
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

Correctly Classified Instances          96               75      %
Incorrectly Classified Instances        32               25      %
Kappa statistic                          0.4711
Mean absolute error                      0.3323
Root mean squared error                  0.4054
Relative absolute error                 67.4955 %
Root relative squared error             81.7155 %
Total Number of Instances              128     


=== Confusion Matrix ===

  a  b   <-- classified as
 30 26 |  a = 0
  6 66 |  b = 1



=== Stratified cross-validation ===

Correctly Classified Instances          92               71.875  %
Incorrectly Classified Instances        36               28.125  %
Kappa statistic                          0.4194
Mean absolute error                      0.3375
Root mean squared error                  0.4111
Relative absolute error                 68.5301 %
Root relative squared error             82.8371 %
Total Number of Instances              128     


=== Confusion Matrix ===

  a  b   <-- classified as
 34 22 |  a = 0
 14 58 |  b = 1

