# Component: Education

## Purpose

Education covers school life: classes and courses, studying, exams, grades,
majors, and the struggles that come with them. It's fifth in the dispatch
order, so a line that's really about crisis, a relationship, money, or
health gets claimed before it reaches here. That's why "I'm stressed about
my exams" is answered by `wellbeing` rather than `education` — the stress is
the more useful thing to respond to, and `wellbeing` sits ahead of this
component.

## Data storage

No state persists between calls. `educationMatches()` and
`educationRespond()` are free functions declared in `education.h` and wired
into the shared `Category` table that `category_registry.cpp` builds.
`respond()` declares nine `static const std::regex` objects that compile once
on first use and are reused on every later call, which is the same approach
the other seven components take.

## Functionality

`educationMatches()` runs one `regex_search` against a keyword alternation
covering class, course, university, college, school, study, student, exam,
midterm, quiz, homework, assignment, professor, lecture, degree, major,
semester, grades, GPA, graduation, and tuition. It also carries the trigger
words the response patterns need in order to fire at all — failed, passed,
struggling, understand, and dropped — because `matches()` gates `respond()`,
so a pattern with no matching keyword can never run.

One of those keywords needed a guard. Adding a bare "passed" would have
routed "my grandmother passed away" into this component and answered it as
if it were an exam result, so the keyword is written as
`passed(?!\s+away)`. The negative lookahead drops that phrasing out of
education entirely, and it falls through to the neutral program-wide
fallback instead.

`educationRespond()` checks specific patterns before general ones. A
whole-line "I'm failing" is caught with `regex_match`. After that come "I
failed X," "I passed X," "I'm struggling with X," "I don't understand X,"
the major patterns, studying, having an exam coming up, and dropping a
class. The capturing patterns pull the subject out with a group, run it
through `text_utils::reflect` so pronouns flip, and build the reply with
`text_utils::fillTemplate`. The major and studying patterns are each written
as two alternatives with a capture group in each ("I'm majoring in X" versus
"my major is X"), so the code checks `match[1].matched` to see which group
actually holds the text. If the keyword check passed but nothing more
specific fired, it returns a general prompt about school.
