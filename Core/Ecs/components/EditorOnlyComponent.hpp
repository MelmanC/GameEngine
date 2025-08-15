#pragma once

#include <string>

namespace ecs {

  struct EditorOnlyComponent {
      bool isEditorOnly = true;
      std::string tags = "";
  };

}  // namespace ecs
