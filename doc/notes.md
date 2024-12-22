- Why use a transition table as an array instead of the traditional method using if/else/switch statements?

    - With this method, we can keep our state/transition code mostly declarative, and boring token checks isolated. This makes maintainance much easier: we end up delegating most of the mental overhead of the parsing operations to a pushdown automaton.

    - Downside: in some complex languages, it might be extremely difficult to model the grammar as an automaton. Make sure your grammar is clean and avoids the usual problems like ambiguity, left recursion, and stuff idk much about grammars lmao

- Make sure the first transition has an error if others don't, so we can return the first error and keep the code simple.

- Like typical recursive descent implementations, we emulate the strategy of every state only verifying the current token. We consume the next token **after** that.

- Special states:

    - Checkpoint state: becomes the new initial state. This is necessary since our grammar specifies an order of "global rules". We don't want to go back to a previous rule after we find an accepting state.

    - Stack state: originally called by me as a "goto" state. This is used to store the context of the caller of the rule, so we can go back to it after we find an accepting state. In this simple grammar, we use a stack of length 1 (variable that is -1 if empty, has the caller state value otherwise), since rules are not nested, just optionally repeated.

- Callback functions: this makes our parser code a little cleaner. We can keep boring and potentially verbose token match checks outside of the critical logic.
