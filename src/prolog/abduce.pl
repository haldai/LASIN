%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Abuduce statistical classifiers
%      AUTHOR: WANG-ZHOU DAI
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Prog = ps(MetaSubs, Signature, SizeBound, MetaRules)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Abductive/inductive proving
prove([], Prog, Prog).
prove([Atom-PostTest | Atoms], Prog1, Prog2):-
    write('dProve'-Atom),
    d_prove([Atom-PostTest], Prog1),
    PostTest, !,
    write('dProve_passed'-Atom-Atoms), nl,
    prove(Atom, Prog1, Prog2).
prove([Atom-Post | Atoms], Prog1, Prog2):-
    metarule(RuleName, MetaSub, (Atom:-Body), PreTest, Prog1),
    write('MetaRule='), write(metarule(RuleName, MetaSub, (Atom:-Body), PreTest)), nl,
    call(PreTest),
    (PostTest == noExampleH ->
	 RuleName \== instance;
     true
    ),
    write('Passed PreTest'), nl,
    write('TRYING CLAUSE:'), nl, printprog([metasub(RuleName, MetaSub)]),
    abduce(metasub(RuleName, MetaSub), Prog1, Prog3), % Binds MetaSub
    Prog3 = ps(_, _, Left, _),
    write('CLAUSE LEFT = '), write(Left), nl,
    prove(Body, Prog3, Prog4),
    PostTest,
    write('Passed PostTest'), nl,
    prove(Atoms, Prog4, Prog2).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Print predicates & clauses
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

printprog(MetaSub):-
    copy_term(MetaSub, MetaSub1),
    converts(MetaSub1, Cs), nl, sort(Cs, Cs1),
    printclauses(Cs1), !.

converts([],[]):-
    !.
converts([metasub(RuleName, MetaSub) | MIs], [Clause | Cs]):-
    metarule(RuleName, MetaSub, Clause, _, _),
    numbervars(Clause, 0, _),
    converts(MIs, Cs), 
    !.

printclauses([]):-
    nl, !.
printclauses([C | Cs]):-
    printclause(C), nl,
    printclauses(Cs).

printclause((Head:-[])):-
    printatom(Head), write('.').
printclause((Head:-Body)) :-
    printatom(Head), write(':-'),
    printatoms(Body).

printatom_cond(List-_):-
    printatom(List).

printatom(List):-
    Atom =.. List, write(Atom).

printatoms([A]):-
    printatom_cond(A),
    write('.'), !.
printatoms([A|As]):-
    printatom_cond(A),
    write(', '),
    printatoms(As), !.
