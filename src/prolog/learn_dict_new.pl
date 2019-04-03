:-['test_ol.pl'].

learn_dict(6):-
    test_go_os2(20, Model, [50, 0.4, 0, 100], kmeans),
    test_go_os2(20, Model, [100, 0.4, 0, 100], stroke).    
learn_dict(8):-
    test_go_os2(20, Model, [100, 0.4, 0, 100], kmeans),
    test_go_os2(20, Model, [100, 0.4, 0, 100], spectral).
learn_dict(10):-
    test_go_os2(10, Model, [200, 0.4, 0, 100], spectral).
learn_dict(11):-
    test_go_os2(10, Model, [200, 0.4, 0, 100], kmeans).
learn_dict(12):-
    test_go_os2(10, Model, [200, 0.4, 0, 100], stroke).
