%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Learn statistical classifiers by abduction
%      AUTHOR: WANG-ZHOU DAI
% ========== User-defined metarules
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/* @TODO:
% Total father rule: because a son may have numbers of parents,
% this metarule should be dynamic.
metarule1(total_father, [P/1 | Body_pred],
          ([P, X]:-[Body_template]), Pre, Prog):-
    % Dynamic body predicates
    findall(Q, father(Q, P), Fathers_), % find all parents
    add_arity(Fathers_, 1, Fathers), % add arity to all parent predicates
    num_mid(Body_rest_, _), % number of the invented predicates
    add_arity(Body_rest_, 1, Body_rest),
    Body_pred = [Fathers | Body_rest],
    % Dynamic body template ([Q, X]-true)
    
    fail.
    %Pre = (pred_above(P/1, Q/1, Prog), pred_above(P/1, R/Ra, Prog)).
*/

% Father constraint rule
% P(X,Data,IN,OUT):-Q(Data,IN,OUT1),R(DATA,OUT1,OUT2).
metarule_(father_con, [P/3, Q/3, R/3],
%          ([P, X]:-[[Q, X]-true, [R, X]-true]), Pre, Prog):-
          ([P, X, R1, R2]:-[[Q, X, R1, R3], [R, X, R3, R2]]), Pre, Prog):-
    father(Q, P),
    % Pre = [pred_above(P/1, Q/1, Prog), pred_above(P/1, R/1, Prog)]. %  @TODO: random order learning
    Pre = (pred_above(P/3, Q/3, Prog), pred_above(P/3, R/3, Prog)). % fixed order learning

% Learn a statistical classifier as new rule
% C is the variable for classifier
/*
% With post test
metarule_(stat_cls, [P/1, stat_cls/2, C], ([P, X]:-[[stat_cls, C, X]-Post]), Pre, Prog):-
    obj_gt(ObjGT), % atom_gt
    Pre = true,
    Post =.. [ObjGT, C, X, Prog]. % atom_gt(C, X, Prog):- C \== X.
*/

metarule_(stat_cls, [P/3, stat_classifier/4, C],
          ([P, X, R1, R2]:-[[stat_classifier, X, R1, C, R2]]), Pre, _):-
    Pre = true.

% control the number of mid-level predicates in rule template
num_mid_pred([], 0).
num_mid_pred([_], 1).



%================================
% for binary classification task
%================================
metarule_(sub_concepts, [S | Mss],
          ([concept, P, Data, In, Out]:-[BODY]), Pre, _):-
    my_findall(S, sub_concept(P, S), SUBs), % find all sub concepts
    SUBs \== [], % at least one sub-concept
    !.

metarule_():-
    !.
              
    
