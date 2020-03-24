#include "AccessibleUtils.h"
#include "loguru.hpp"

char* state_to_char(AtspiStateType state)
{
   switch (state) {
   case ATSPI_STATE_INVALID:
      return strdup("ATSPI_STATE_INVALID");
   case ATSPI_STATE_ACTIVE:
      return strdup("ATSPI_STATE_ACTIVE");
   case ATSPI_STATE_ARMED:
      return strdup("ATSPI_STATE_ARMED");
   case ATSPI_STATE_BUSY:
      return strdup("ATSPI_STATE_BUSY");
   case ATSPI_STATE_CHECKED:
      return strdup("ATSPI_STATE_CHECKED");
   case ATSPI_STATE_COLLAPSED:
      return strdup("ATSPI_STATE_COLLAPSED");
   case ATSPI_STATE_DEFUNCT:
      return strdup("ATSPI_STATE_DEFUNCT");
   case ATSPI_STATE_EDITABLE:
      return strdup("ATSPI_STATE_EDITABLE");
   case ATSPI_STATE_ENABLED:
      return strdup("ATSPI_STATE_ENABLED");
   case ATSPI_STATE_EXPANDABLE:
      return strdup("ATSPI_STATE_EXPANDABLE");
   case ATSPI_STATE_EXPANDED:
      return strdup("ATSPI_STATE_EXPANDED");
   case ATSPI_STATE_FOCUSABLE:
      return strdup("ATSPI_STATE_FOCUSABLE");
   case ATSPI_STATE_FOCUSED:
      return strdup("ATSPI_STATE_FOCUSED");
   case ATSPI_STATE_HAS_TOOLTIP:
      return strdup("ATSPI_STATE_HAS_TOOLTIP");
   case ATSPI_STATE_HORIZONTAL:
      return strdup("ATSPI_STATE_HORIZONTAL");
   case ATSPI_STATE_ICONIFIED:
      return strdup("ATSPI_STATE_ICONIFIED");
   case ATSPI_STATE_MULTI_LINE:
      return strdup("ATSPI_STATE_MULTI_LINE");
   case ATSPI_STATE_MULTISELECTABLE:
      return strdup("ATSPI_STATE_MULTISELECTABLE");
   case ATSPI_STATE_OPAQUE:
      return strdup("ATSPI_STATE_OPAQUE");
   case ATSPI_STATE_PRESSED:
      return strdup("ATSPI_STATE_PRESSED");
   case ATSPI_STATE_RESIZABLE:
      return strdup("ATSPI_STATE_RESIZABLE");
   case ATSPI_STATE_SELECTABLE:
      return strdup("ATSPI_STATE_SELECTABLE");
   case ATSPI_STATE_SELECTED:
      return strdup("ATSPI_STATE_SELECTED");
   case ATSPI_STATE_SENSITIVE:
      return strdup("ATSPI_STATE_SENSITIVE");
   case ATSPI_STATE_SHOWING:
      return strdup("ATSPI_STATE_SHOWING");
   case ATSPI_STATE_SINGLE_LINE:
      return strdup("ATSPI_STATE_SINGLE_LINE");
   case ATSPI_STATE_STALE:
      return strdup("ATSPI_STATE_STALE");
   case ATSPI_STATE_TRANSIENT:
      return strdup("ATSPI_STATE_TRANSIENT");
   case ATSPI_STATE_VERTICAL:
      return strdup("ATSPI_STATE_VERTICAL");
   case ATSPI_STATE_VISIBLE:
      return strdup("ATSPI_STATE_VISIBLE");
   case ATSPI_STATE_MANAGES_DESCENDANTS:
      return strdup("ATSPI_STATE_MANAGES_DESCENDANTS");
   case ATSPI_STATE_INDETERMINATE:
      return strdup("ATSPI_STATE_INDETERMINATE");
   case ATSPI_STATE_REQUIRED:
      return strdup("ATSPI_STATE_REQUIRED");
   case ATSPI_STATE_TRUNCATED:
      return strdup("ATSPI_STATE_TRUNCATED");
   case ATSPI_STATE_ANIMATED:
      return strdup("ATSPI_STATE_ANIMATED");
   case ATSPI_STATE_INVALID_ENTRY:
      return strdup("ATSPI_STATE_INVALID_ENTRY");
   case ATSPI_STATE_SUPPORTS_AUTOCOMPLETION:
      return strdup("ATSPI_STATE_SUPPORTS_AUTOCOMPLETION");
   case ATSPI_STATE_SELECTABLE_TEXT:
      return strdup("ATSPI_STATE_SELECTABLE_TEXT");
   case ATSPI_STATE_IS_DEFAULT:
      return strdup("ATSPI_STATE_IS_DEFAULT");
   case ATSPI_STATE_VISITED:
      return strdup("ATSPI_STATE_VISITED");
   case ATSPI_STATE_CHECKABLE:
      return strdup("ATSPI_STATE_CHECKABLE");
   case ATSPI_STATE_HAS_POPUP:
      return strdup("ATSPI_STATE_HAS_POPUP");
   case ATSPI_STATE_READ_ONLY:
      return strdup("ATSPI_STATE_READ_ONLY");
   case ATSPI_STATE_LAST_DEFINED:
      return strdup("ATSPI_STATE_LAST_DEFINED");
   case ATSPI_STATE_MODAL:
      return strdup("ATSPI_STATE_MODAL");
   default:
      return strdup("\0");
   }
}

static void debug( AtspiStateSet *stateSet)
{
   if (!stateSet) return;
   GArray *states = atspi_state_set_get_states(stateSet);
   if (!states) return;

   char *state_name = NULL;
   AtspiStateType stat;
   LOG_F(INFO, "check ss:%p s:%p, len:%d", stateSet, states, states->len);
   for (int i = 0; states && (i < states->len); ++i) {
      stat = g_array_index(states, AtspiStateType, i);
      state_name = state_to_char(stat);
      LOG_SCOPE_F(INFO, "state: %s", state_name);
      free(state_name);
   }
   g_array_free(states, 1);
}