
Naive Bayes Classifier

                Class
Attribute           0      1
               (0.44) (0.56)
=============================
index0
  0               17.0   49.0
  1               41.0   25.0
  [total]         58.0   74.0

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

Correctly Classified Instances         104               81.25   %
Incorrectly Classified Instances        24               18.75   %
Kappa statistic                          0.625 
Mean absolute error                      0.2932
Root mean squared error                  0.3738
Relative absolute error                 59.563  %
Root relative squared error             75.3554 %
Total Number of Instances              128     


=== Confusion Matrix ===

  a  b   <-- classified as
 48  8 |  a = 0
 16 56 |  b = 1



=== Stratified cross-validation ===

Correctly Classified Instances         104               81.25   %
Incorrectly Classified Instances        24               18.75   %
Kappa statistic                          0.625 
Mean absolute error                      0.3003
Root mean squared error                  0.3818
Relative absolute error                 60.9756 %
Root relative squared error             76.9323 %
Total Number of Instances              128     


=== Confusion Matrix ===

  a  b   <-- classified as
 48  8 |  a = 0
 16 56 |  b = 1

