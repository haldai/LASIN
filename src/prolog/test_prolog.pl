:- load_foreign_library(foreign('../test_cpp_prolog/test_prolog.so')).

valid(A, B):-
    write('Start validation!\n'),
    mat_prod(A, B, C),
    write('Product OK!\n'),
    write(C),
    write('\n'),
    mat_valid(C).

%tl(X):-
%    make_list(X, [0, 1, 4, 9, 16, 25, 36, 49, 64, 81]).

tl(X):-
    make_list(X, Y),
    write(Y), nl.
%    Y == [0.0, 1.0, 4.0].

un(X):-
    write(X),
    nl,
    (X == [0.0,1.0,2.0] ->
         (write('TRUE!'), nl, !);
     (write('FALSE!'), nl, !)
    ).

test_cls(X):-
    write(X), nl,
    test_classifier(X, Y).
