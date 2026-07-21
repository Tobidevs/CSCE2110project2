# Component: Entertainment

## Purpose

Entertainment handles what the user does for enjoyment: concerts, movies and
shows, music, reading, dancing, restaurants and bars, and going out. It sits
sixth in the dispatch order. Video games are deliberately left out of its
keyword list and handled by `technology` instead, since that component
already owns screen time and the two would otherwise compete for the same
lines.

## Data storage

No state persists between calls. `entertainmentMatches()` and
`entertainmentRespond()` are free functions declared in `entertainment.h`.
`respond()` builds eight `static const std::regex` objects on first call and
reuses them afterward, matching the pattern used across the other
components. Nothing is shared with any other component except the
`text_utils` helpers and the `Category` record in the registry.

## Functionality

`entertainmentMatches()` runs one `regex_search` against a keyword
alternation covering concert, movie, film, show, TV, music, band, song,
album, dancing, restaurant, bar, club, party, festival, theater, museum,
book, reading, listening, watching, hobbies, and going out.

`entertainmentRespond()` orders its patterns most-specific-first. The
favorites pattern runs before everything else because it captures two groups
at once — the kind of thing ("movie," "song," "band") and its title — and
builds the reply by calling `text_utils::fillTemplate` twice, once per
placeholder. Next is a pattern for having gone somewhere, which captures the
venue word itself rather than the rest of the sentence so the reply stays
clean.

The remaining patterns cover wanting to see or watch something, having
watched something, listening to music, reading, dancing, and going out.
"I want to see X" is checked before "I saw X" so the intent version isn't
swallowed by the past-tense rule. Captured text runs through
`text_utils::reflect` before being substituted, so pronouns flip the way
they do everywhere else in the program. If the keyword check passed but no
specific pattern matched, the component returns a general prompt rather than
nothing, which is what keeps the program's guarantee of responding to every
input intact.
