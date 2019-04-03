<TeXmacs|1.99.4>

<style|article>

<\body>
  \;

  <doc-data|<doc-title|Abduction feature relationship with logic>>

  <section|Problem>

  <strong|Input:>

  <\itemize>
    <item>Training data: <math|<around*|(|\<b-x\><rsub|i>,y<rsub|i>|)>>,
    <math|x> is feature, <math|y=1 or 0> is label (maybe multiple labels)

    <item>Background knowledge:\ 

    <\itemize>
      <item>BK in domain, e.g.:

      <\itemize>
        <item>Domain has concepts <verbatim|con1,con2,...>

        <item>A concept <verbatim|con_i> only affect features of indices
        <verbatim|[f_1,f_2,...]>.

        <item>some instances are constructed from some specific concepts:
        e.g. <verbatim|inst_i> has concept <verbatim|con_j>.

        <item>relations between sub-concepts, e.g.:

        exlcusiveness: <verbatim|exclusive(con_i,con_j)>,

        cooccurance: <verbatim|co_occur(con_i,con_j)>,

        spatial relationship: <verbatim|left_of(con_i,con_j)>,

        ...
      </itemize>

      <item>Meta knowledge:\ 

      <\itemize>
        <item>template (meta-rules) for rule abduction; knowledge of
        constructing meta-rules. For example,
        <verbatim|father(con_i,con_j)><math|\<rightarrow\>><verbatim|metarule(con_i(X):-con_j(X),R(X))>,
        meaning if concept <verbatim|con_i> is father of concept
        <verbatim|con_j>, then every rule about <verbatim|con_i> must include
        <verbatim|con_j> in its body.

        <item>primitives of statistical models, e.g.:

        <verbatim|cluster(Data,Num_Centeres,Partition)>,

        <with|color|red|<verbatim|sparse_coder(Data,Dict)>,>

        <verbatim|stat_classifier(Data,Labels,Model,Predictions)>,

        <verbatim|data_feature_filter(Data,Feature_Idxs,New_Data)>,

        <verbatim|data_instance_filter(Data,Data_Idxs,New_Data)>,

        ...

        \;
      </itemize>
    </itemize>
  </itemize>

  <no-indent><strong|Output:>

  <\itemize>
    <item>A set of rules as hypothesis for target concept <verbatim|label>.

    <item>A set of rules to describe relations between concepts in domain
    (for transferability?)
  </itemize>

  <section|Intuition>

  The number of relations is exponential to the number of concepts, each
  concept is affected by plenty of feature, so there will be a huge number of
  relations in feature space, and most of them are unimportant. <strong|Using
  backgound knowledge to abduce feature relationships enables us to focus on
  relationships that only related to background knowledge, which are more
  important and comprehensible to the users>.

  <subsection|Main Idea>

  <strong|Example>:

  When learning the concept of automobile, we have background knowledge that
  there are 2 sub-concepts <verbatim|A> and <verbatim|B> belong to
  <verbatim|automobile>, and relation that <verbatim|A> always lies on the
  bottom of an <verbatim|automobile> and just below (and connected with)
  <verbatim|B>. After showing some examples of <verbatim|automobile>, one can
  easily construct a valid hypothesis that <verbatim|A> should be wheel,
  <verbatim|B> should be car's body.\ 

  Moreover, this hypothesis will also be helpful when this person is trying
  to understand the concept of bicycle, since the knolwedge of <verbatim|A>
  and <verbatim|B> can be direclty transfered.\ 

  \;

  <strong|<with|color|blue|Background knowledge
  <math|\<Longrightarrow\><rsup|<small|<tiny|\<spadesuit\>>>>> Meta-rules
  (template rules) <math|\<Longrightarrow\>> Statistical Models
  learning.><strong|>>

  <small|<yes-indent><space|1em><math|<rsup|<tiny|\<spadesuit\>>>\<Longrightarrow\>>
  means \Pabduce\Q>

  <\itemize>
    <item>Incorporating first-order background knowledge by letting them to
    control the learning of statistical classifiers. The meta-rules act as
    templates and enables predicate invention, and the invented
    \Ppredicates\Q are statistical classifiers.

    <item>Statistical learning is directly controled by its training data,
    which is actually controlled by rule induction by variable bindings in
    primitives from background knowledge.

    <item>Reuse the learned classifiers as logical predicates. The invented
    predicate (for exapmle the <verbatim|R> in last paragraph) might be an
    useful mid-level concept that can help the learning of other labels.
  </itemize>

  <subsection|Main Challenge>

  <strong|Semantic Gap>: Relations in human knowledge are mostly defined in a
  semantic space (between high-level concepts). However, in statsitcal
  learning, most data are located in a primary low-level feature space, which
  is far from semantic meaningful. <em|Semantic gap> is the difference
  between human possessed knowledge space and statsitcal learning data's
  feature space. <strong|For the above \Pautomobile's wheels and body\Q
  example, the challenge is how to split \Pwheels\Q and \Pcar body\Q in the
  feature space and understand they are two different objects that may have
  relationship.>

  Typically, most datasets are collected in a statistical way, i.e., in each
  dimension of a feature vector, the values are statistics of each attribute.
  For example, TFIDF/bag of words features records the number of appearance
  of a word. This kind of features only preserve appearance of attributes, so
  only numerical and statistical relations can be defined, e.g.
  <verbatim|\<gtr\>>, <verbatim|\<less\>>, <verbatim|co_occurance>, etc.
  Furthermore, extracting low-level features may cause loss of many semantic
  relations. Such as in computer vision domain, after feature extraction
  (HOG, SIFT, CNN, etc.), images are encoded into vectors, the loss of
  spatial information causes the incapability of spatial relations like
  <verbatim|above> and <verbatim|below>.

  To solve the semantic gap problem and implant background knowledge in
  statistical machine learning process, we may have 2 kinds of solutions: 1)
  design background knowledge in mathematical & statistical language and make
  it applicable on statistical feature space; 2) induce high-level concepts
  from statistical feature space and direclty apply ordinary human knowledge.

  <subsection|Possible ideas>

  <subsubsection|<with|color|red|<strong|[Deprecated]>> symbols as graphical
  model variables (Generative)>

  <yes-indent>The <strong|structure of feature space> is assumed like this:

  <\with|par-mode|center>
    <small-figure|<image|feature_structure.eps|400px|||>|>
  </with>

  Data <math|<around*|(|\<b-x\><rsub|i>,y<rsub|i>|)>> is generated by a
  probabilistic model, where <math|\<b-x\>=x<rsub|1>,\<ldots\>.,x<rsub|n>> is
  feature vector, <math|y\<in\><around*|{|0,1|}>> is label. For example in
  Figure 1, suppose there is only one <em|target concept> (label)
  <verbatim|L>, it has 2 <em|sub-concept>s <verbatim|H1> and <verbatim|H2>.
  Each sub-concept can be regarded as a <em|latent label>, target/sub
  concepts controls the generation of each <em|feature> <math|X<rsub|i>>. In
  the example above (if internal sub-concepts have no connections), the
  generative model is:

  <\equation*>
    P<around*|(|L,H1,H2,X1,X2,X3|)>=P<around*|(|L|)>P<around*|(|H1<around*|\||L|\<nobracket\>>|)>P<around*|(|H2<around*|\||L|\<nobracket\>>|)>P<around*|(|X1<around*|\||H1|\<nobracket\>>|)>P<around*|(|X2<around*|\||H1,H2|\<nobracket\>>|)>P<around*|(|X3<around*|\||H2|\<nobracket\>>|)>
  </equation*>

  <yes-indent>In order to learn this structure, certain background knowledge
  (even some annotations) is needed. For example, oracle can provide
  evidences of which features does the concept controls; or annotations that
  tells which concept does an instance belongs to.\ 

  <yes-indent>The key idea of this model is \P<strong|target concept is a
  combination of its sub-concepts, and each (sub-)concept only affects a
  subspace in original feature space>\Q. The knowledge of concepts and
  corresponding feature space are given in first-order logical form.

  <no-indent>

  <with|color|red|<no-indent><strong|PROBLEM:>>

  If a feature is generated from multiple concepts, the joint probability
  would be difficult to model (like the <math|X<rsub|2>> in the above
  example). <strong|We need strong heuristics as prior to model the
  conditional probabilities.> For example mixture gaussian or additivity [A.
  Anandkumar et. al., 2013, <em|Learning Linear Bayesian Networks with Latent
  Variables>].

  <no-indent><with|color|red|<strong|REASON OF GIVING UP THIS IDEA:>>

  This algorithm doesn't work well because I hardly cand find a general way
  to model conditional probability well (even on artifitial data); The
  feature structure is restricted to <verbatim|is_a(A,B)>,
  <verbatim|exclusive(A,B)> kind of relationships.

  <subsubsection|<with|color|green|<strong|[Undergoing]>> symbols as feature
  dictionary (Discriminative)>

  <yes-indent><no-indent>There is no specific assumption on the structure of
  domain. A concept is considered as a vector (or a set of vectors) in the
  original feature space. For example, handwritten characters and numbers are
  constructed by pen strokes, each stroke could be seen as a sub-concept of a
  character and represented by a vector of pixels. When writing a character,
  human is writing strokes follow a certain convention, e.g. a fixed orders
  and fixed patterns. The orders and patterns are structures in the feature
  space of pen strokes. Popular feature learning techniques do not consider
  this structure, most extracted features are scattered and meaningless,
  which makes the structure unclear.

  A straight forward idea is using sparse coding to learn a dictionary from
  the original feature space, then use background knowledge to examine
  whether does the learned symbols (item in dictionary) satisfy the
  relational constraints in backgound knowledge. <with|color|red|The main
  problem is that the dictionary learned by sparse coding strongly depends on
  its initialization (sampled data patches and initial values of dictionary),
  we cannot guarantee the learned dictionary contains the desired logical
  symbols in background knowledge.> In short, the smantic gap will result in
  the difference between the learned dictionary and the logical symbols in
  background knowledge. (Although all sparse coding and autoencoder works
  claim that the learned dictionary presumes semantical meaningful concepts,
  but most of them are still too primary to form first-order logic relations,
  and the appearance of those concepts in learned dictionary cannot be
  guaranteed).

  To solve this problem, we propose a repeated hypothesis then revise
  procedure. Main steps:

  <\enumerate>
    <item>Sample a sub-part from each data;

    <item>Use the sub-parts as training data patches to train a sparse coder;

    <item>Use the trained sparse coder to reconstruct original data;

    <item>Get the difference between orignal data and reconstructed data;

    <item><with|color|red|Use the difference and certain background knowledge
    to resample sub-parts> as data patches, goto step 2.
  </enumerate>

  <strong|Conclusion:>

  This is a <strong|unsupervised(?)> learning framework (no labels, but with
  other kind of backgound knolwdges), it can learn dictionary whose items
  follows a structure defined by logical language.

  <section|Language to describe relations in feature space>

  In this section we introduce the language for describing background
  knowledge in a binary classification problem.

  <\itemize>
    <item>Primitives:

    <\itemize>
      <item><verbatim|concept(Con,Vector)>: A concept is a vector (basis) in
      feature space

      <item><verbatim|Relation(Con1,Con2)>: <verbatim|Con1> and
      <verbatim|Con2> has certain relations, e.g.,\ 

      <\itemize>
        <item><verbatim|move_left(Con1, Con2)> means <verbatim|Con2> can be
        obtained by moving <verbatim|Con1> to the left (on a image).
      </itemize>

      <\itemize>
        <item><verbatim|exclusive(Con1,Con2)>: <verbatim|Con1> and
        <verbatim|Con2> never appear together.
      </itemize>

      <\itemize>
        <item><verbatim|co_occur(Con1,Con2)>: <verbatim|Con1> and
        <verbatim|Con2> always occur together.
      </itemize>

      <item>... ([Probabilistic] label relation constraints, to be added)

      <item>Statsitical models:

      <\itemize>
        <item><verbatim|sparse_coder(Data, Param, Model, Code)>: Learn a
        sparse coding dicitionary (stored in <verbatim|Model>), and encode
        original data with the learned dictionary.
      </itemize>
    </itemize>

    <item>Target Hypotheses: After learning, we want to obtain hypothesis
    like this:

    <\itemize>
      <item>A <verbatim|Model> defining coding dictionary
      <math|d<rsub|1>,d<rsub|2>,\<ldots\>d<rsub|D>>

      <item>A set of facts that describing feature relationship:
      <verbatim|rel(d_1, d_3),...>
    </itemize>
  </itemize>

  <section|MNIST example>

  Using <math|1\<times\>748> original pixel directly (not sampling
  <math|14\<times\>14> windows on original data, which can only learn small
  strokes having no semantic relations)

  <\itemize>
    <item>If we do not include background knowledge, the learned dicts are
    whole picture of numbers themselves. <with|color|green|[done]>

    <item>If we use the proposed method, we can use spatial background
    knowledge (a number could be split to \Pup and down\Q two parts, etc.),
    it should return a dictionary with number structures (e.g. \P9\Q is
    composed by a small \Po\Q on the up and a line or a curve down it)
    <with|color|blue|[doing]>
  </itemize>

  \;
</body>

<\initial>
  <\collection>
    <associate|page-type|a4>
  </collection>
</initial>

<\references>
  <\collection>
    <associate|auto-1|<tuple|1|?>>
    <associate|auto-10|<tuple|4|?>>
    <associate|auto-2|<tuple|2|?>>
    <associate|auto-3|<tuple|2.1|?>>
    <associate|auto-4|<tuple|2.2|?>>
    <associate|auto-5|<tuple|2.3|?>>
    <associate|auto-6|<tuple|2.3.1|?>>
    <associate|auto-7|<tuple|1|?>>
    <associate|auto-8|<tuple|2.3.2|?>>
    <associate|auto-9|<tuple|3|?>>
  </collection>
</references>

<\auxiliary>
  <\collection>
    <\associate|figure>
      <tuple|normal||<pageref|auto-7>>
    </associate>
    <\associate|toc>
      <vspace*|1fn><with|font-series|<quote|bold>|math-font-series|<quote|bold>|1<space|2spc>Problem>
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-1><vspace|0.5fn>

      <vspace*|1fn><with|font-series|<quote|bold>|math-font-series|<quote|bold>|2<space|2spc>Intuition>
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-2><vspace|0.5fn>

      <with|par-left|<quote|1tab>|2.1<space|2spc>Main Idea
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-3>>

      <with|par-left|<quote|1tab>|2.2<space|2spc>Main Challenge
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-4>>

      <with|par-left|<quote|1tab>|2.3<space|2spc>Possible ideas
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-5>>

      <with|par-left|<quote|2tab>|2.3.1<space|2spc><with|color|<quote|red>|<with|font-series|<quote|bold>|math-font-series|<quote|bold>|[Deprecated]>>
      symbols as graphical model variables (Generative)
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-6>>

      <with|par-left|<quote|2tab>|2.3.2<space|2spc><with|color|<quote|green>|<with|font-series|<quote|bold>|math-font-series|<quote|bold>|[Undergoing]>>
      symbols as feature dictionary (Discriminative)
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-8>>

      <vspace*|1fn><with|font-series|<quote|bold>|math-font-series|<quote|bold>|3<space|2spc>MNIST
      example> <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-9><vspace|0.5fn>

      <vspace*|1fn><with|font-series|<quote|bold>|math-font-series|<quote|bold>|4<space|2spc>Meta
      Language> <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-10><vspace|0.5fn>
    </associate>
  </collection>
</auxiliary>