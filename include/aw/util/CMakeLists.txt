add_subdirectory(containers)
add_subdirectory(file)
add_subdirectory(log)
add_subdirectory(math)
add_subdirectory(message)
add_subdirectory(reflection)
add_subdirectory(serialization)
add_subdirectory(time)
add_subdirectory(type)

add_to_target(awUtil PUBLIC color.hpp)
add_to_target(awUtil PUBLIC colors.hpp)
add_to_target(awUtil PUBLIC types.hpp)
