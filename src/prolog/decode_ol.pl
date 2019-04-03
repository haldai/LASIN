:- ['test.pl'].

go1 :-
   test_decode('/home/lamda/Storage_SV52/daiwz/abduce/results/org_200/data_100/dict.csv',
               [200, 0.4, 0, 100],
               '../../data/omniglot_test.csv',
               '/home/lamda/Storage_SV52/daiwz/abduce/results/org_200/data_100/omniglot_test_code.csv').
go2:-   
   % strokes
   test_decode('/home/lamda/Storage_SV52/daiwz/abduce/results/stroke_200/data_100/dict_6.csv',
               [200, 0.4, 0, 100],
               '../../data/omniglot_test.csv',
               '/home/lamda/Storage_SV52/daiwz/abduce/results/stroke_200/data_100/omniglot_test_code.csv').
go3:-
   % kmeans
   test_decode('/home/lamda/Storage_SV52/daiwz/abduce/results/kmeans_200/data_100/M100_dict_1.csv',
               [200, 0.4, 0, 100],
               '../../data/omniglot_test.csv',
               '/home/lamda/Storage_SV52/daiwz/abduce/results/kmeans_200/data_100/omniglot_test_code.csv').
go4:-   % spectral
   test_decode('/home/lamda/Storage_SV52/daiwz/abduce/results/spectral_200/data_100/M100_dict_1.csv',
               [200, 0.4, 0, 100],
               '../../data/omniglot_test.csv',
               '/home/lamda/Storage_SV52/daiwz/abduce/results/spectral_200/data_100/omniglot_test_code.csv').
  
