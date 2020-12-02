#include "binom/includes/structure/variables/variable.h"

int main() {
  using namespace binom;

  using T = VarType;

  Variable matrix =
  mtrx{
  {T::qword, T::byte_array, T::byte_array, T::byte, T::object},
  {
          0_ui64, "admin", "admin", 0xff_ui8, obj{},
          1_ui64, "guest", "guest", 0x00_ui8, obj{},
          2_ui64, "sh13max", "ifvfyvfrc13", 0xff_ui8, obj{}
  }};

  Variable table =
  tbl {
  { {T::qword, "id"}, {T::byte_array, "login"}, {T::byte_array, "password"}, {T::byte, "access_lvl"}, {T::object, "options"} },
  {
          0_ui64, "admin", "admin", 0xff_ui8, obj{},
          1_ui64, "guest", "guest", 0x00_ui8, obj{},
          2_ui64, "sh13max", "ifvfyvfrc13", 0xff_ui8, obj{}
  }
  };

  return 0;
}
