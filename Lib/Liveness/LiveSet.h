#ifndef _V3DLIB_LIVENESS_LIVESET_H_
#define _V3DLIB_LIVENESS_LIVESET_H_
#include <string>
//#include <set>
#include <vector>
#include "UseDef.h"

namespace V3DLib {

class Liveness;
class RegUsage;


#if 0
// Implementation using std::set

/**
 * A live set contains the variables that are live-in to an instruction.
 */
class LiveSet : public std::set<RegId> {
  using Parent = std::set<RegId>;
public:
  void add_not_used(LiveSet const &def, UseDef const &use);
  void add(LiveSet const &rhs);
  void add(Set<RegId> const &rhs);
  std::string dump() const;
  bool member(RegId rhs) const;
};
#endif


/**
 * A live set contains the variables that are live-in to an instruction.
 */
class LiveSet : private std::vector<bool> {
  using Parent = std::vector<bool>;
public:

  /**
   * I hate this so much.
   * All I wanted is clean code with ranges fors, and then I have to deal with waste overhead like this.
   */
  class const_it {
  public:
    const_it(LiveSet const &set);
    const_it(LiveSet const &set, bool set_end);

    bool operator==(const_it const &rhs) const;
    bool operator!=(const_it const &rhs) const { return !(*this == rhs); }

    const_it &operator++();
    RegId operator*() const { return m_cur; }

  private:
    RegId    m_cur;
    LiveSet const &m_set;

    void next();
  };

  const_it begin() const { return const_it(*this); }
  const_it end() const { return const_it(*this, true); }

  LiveSet() = default;
  LiveSet(int size) { Parent::resize(size); }

  void resize(int size);
  void insert(RegId id);
  void clear();
  bool member(RegId rhs) const;
  void add(LiveSet const &rhs);
  void add(Set<RegId> const &rhs);
  void add_not_used(LiveSet const &def, UseDef const &use);
  bool no_items() const;
  std::string dump() const;
};


class LiveSets {
public:
  UseDef useDefSet;

  LiveSets(int size);
  ~LiveSets();

  void init(Instr::List &instrs, Liveness &live);
  LiveSet &operator[](int index);
  std::vector<bool> possible_registers(int index, RegUsage &alloc, RegTag reg_tag = REG_A);

  static RegId choose_register(std::vector<bool> &possible, bool check_limit = true);  
  static void  dump_possible(std::vector<bool> &possible, int index = -1);

  std::string dump2() const;

private:
  int m_size = 0;
  LiveSet *m_sets = nullptr;
};

}  // namespace V3DLib

#endif  //  _V3DLIB_LIVENESS_LIVESET_H_