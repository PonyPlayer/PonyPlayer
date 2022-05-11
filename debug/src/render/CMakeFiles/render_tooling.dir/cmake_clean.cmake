file(REMOVE_RECURSE
  "shader/fragment.fsh"
  "shader/vertex.vsh"
  "test.jpeg"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/render_tooling.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
