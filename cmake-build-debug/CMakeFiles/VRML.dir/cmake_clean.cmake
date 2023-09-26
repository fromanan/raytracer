file(REMOVE_RECURSE
  "VRML.lib"
  "VRML.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/VRML.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
