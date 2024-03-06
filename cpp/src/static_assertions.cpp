#include <limits>
#include <type_traits>

static_assert(
  '0' - '0' == 0,
  "Platform does not have sequential character representations");
static_assert(
  '1' - '0' == 1,
  "Platform does not have sequential character representations");
static_assert(
  '2' - '0' == 2,
  "Platform does not have sequential character representations");
static_assert(
  '3' - '0' == 3,
  "Platform does not have sequential character representations");
static_assert(
  '4' - '0' == 4,
  "Platform does not have sequential character representations");
static_assert(
  '5' - '0' == 5,
  "Platform does not have sequential character representations");
static_assert(
  '6' - '0' == 6,
  "Platform does not have sequential character representations");
static_assert(
  '7' - '0' == 7,
  "Platform does not have sequential character representations");
static_assert(
  '8' - '0' == 8,
  "Platform does not have sequential character representations");
static_assert(
  '9' - '0' == 9,
  "Platform does not have sequential character representations");

// `'9' + 1` must not overflow (if `char` is signed)
static_assert(std::is_signed_v<char>
                ? '9' != std::numeric_limits<char>::max()
                : true,
              "'9' is the maximum value of signed character type - what "
              "platform even is this??");

// will fail to compile if executing
// `'9' + 1` undefined behavior for any other reason
// (execution of undefined behavior during compilation is prohibited)
static_assert('9' + 1 == 1 + '9',
              "Platform is using some kind of weird char representation - "
              "what platform even is this??");

// constraints on representation of '_'
static_assert(('_' - '0') + '0' == '0' + ('_' - '0'));
static_assert(('_' - '0' < 0) || ('_' - '0' > 9));
