file(REMOVE_RECURSE
  "shader/fragment.fsh"
  "shader/vertex.vsh"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/player_tooling.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
