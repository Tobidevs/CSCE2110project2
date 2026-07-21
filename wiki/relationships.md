# Component: Relationships

## Purpose

Relationships covers love and romantic partners: loving someone, a
girlfriend/boyfriend/spouse doing something, missing someone, breaking up,
getting married. It sits right after `concern` in the dispatch order so
ordinary lines like "I love my sister" or "my boyfriend never listens" get a
real response here instead of falling all the way down to `generic_verbs`.

## Data storage

No state persists between calls. `relationshipsMatches()` and
`relationshipsRespond()` are free functions declared in `relationships.h`.
`respond()` builds seven `static const std::regex` objects the first time
it's called and reuses them afterward, the same as `generic_verbs` and
`concern`. There's no dedicated struct beyond the shared `Category` record
that `category_registry.cpp` uses to hold this component's function
pointers.

## Functionality

`relationshipsMatches()` runs one `regex_search` against a keyword
alternation covering the PRD's examples (love, spouse, girlfriend,
boyfriend) plus partner, marriage, divorce, breakup, and dating.

`relationshipsRespond()` checks the most specific patterns first. "I don't
love X" is checked before the plain "I love X" pattern so the negation
doesn't get swallowed by the broader rule, the same trick `generic_verbs`
uses for "don't have" versus "have." One pattern captures "my
girlfriend/boyfriend/spouse/... <predicate>" as two groups, the relation
word and whatever was said about them, and rebuilds a question from both.
Other patterns handle "I miss X," breakups (either "we broke up" with no
named object, or "I broke up with X" — the capture group is optional and
checked with `match[1].matched`), a whole-line "I'm in love" caught with
`regex_match`, and "I'm getting married"/"I got married." If the keyword
check passed but nothing more specific fired, like a bare mention of
"dating," it falls back to a generic prompt.
