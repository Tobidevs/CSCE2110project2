// category_registry.cpp -- builds the priority-ordered table of categories.
// The one file allowed to include every component's header.

#include "category_registry.h"

#include "concern.h"
#include "relationships.h"
#include "financial.h"
#include "wellbeing.h"
#include "education.h"
#include "entertainment.h"
#include "technology.h"
#include "generic_verbs.h"

std::vector<Category> buildCategoryRegistry() {
    // keep concern first so crisis language never gets caught by a broader
    // category's regex first, and generic_verbs last since it's the widest net
    return {
        {"concern", concernMatches, concernRespond},
        {"relationships", relationshipsMatches, relationshipsRespond},
        {"financial", financialMatches, financialRespond},
        {"wellbeing", wellbeingMatches, wellbeingRespond},
        {"education", educationMatches, educationRespond},
        {"entertainment", entertainmentMatches, entertainmentRespond},
        {"technology", technologyMatches, technologyRespond},
        {"generic_verbs", genericVerbsMatches, genericVerbsRespond},
    };
}
