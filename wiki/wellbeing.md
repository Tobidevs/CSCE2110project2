# Component: Wellbeing

## Purpose

Wellbeing covers health habits and how the user is physically feeling:
exercise, sleep, stress, vitamins and diet, relaxing, and being sick. It's
fourth in the dispatch order, so anything that reads as crisis, relationship,
or money talk is claimed before it gets here. That ordering shows up in a
line like "I can't afford my gym membership," which `financial` takes even
though it mentions the gym.

## Data storage

No state persists between calls. `wellbeingMatches()` and
`wellbeingRespond()` are free functions declared in `wellbeing.h`.
`respond()` builds eight `static const std::regex` objects on first call and
reuses them, matching the approach in the other components.

## Functionality

`wellbeingMatches()` runs one `regex_search` against a keyword alternation
covering exercise, workout, gym, jogging, yoga, sleep, insomnia, tired,
exhausted, naps, vitamins, diet, nutrition, health, stress, relax,
meditation, doctor, and sick.

`wellbeingRespond()` goes specific-to-general. A whole-line "I'm tired" or
"I'm exhausted" is caught with `regex_match`, optionally allowing "so" or
"really" in the middle. Sleep gets two more patterns: one for not being able
to sleep at all, and one that captures a number of hours ("I only sleep 4
hours a night") and echoes the figure back. Stress is captured with an
object group so the reply names what the user is stressed about.

Exercise is split into two patterns and the order between them matters.
"I want/need/should/have to work out" is checked first and gets a response
about what's blocking the user; the plain-habit pattern ("I exercise,"
"I go to the gym," "I jog") is checked second and asks how they feel
afterward. Two more patterns cover taking vitamins or a supplement, and
being sick or having seen a doctor. If the keyword check passed but nothing
more specific fired, it returns a general prompt.
