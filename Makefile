INCDEPS := 1
#DEBUG := 1

### 用户变量 ##################################################################
override __version :=
ifdef DEBUG
override __version := -dbg$(__version)
endif
##
project_name := antiLeech
## 三选一: lib 静态库、 dll 动态库、 bin 可执行文件
target_type := dll
##
dist_dir := dist
dist_file := $(dist_dir)/$(project_name).tar.xz
## 安装目录
install_dir := $$HOME/.aMule
## .cpp 文件的路径
src_files :=
## .cpp 文件的目录
src_root_dir := src
src_dirs :=
## .o 文件的目录
obj_dir :=
## .hpp 文件的目录
inc_dirs := src
## 需要连接的库的名字
lib_names :=
## 需要连接的库的目录
lib_dirs :=
## 编译器的标志选项
flags := -fPIC -O2
# flags += -Wall -Werror -Wextra
## 预编译器的宏
defines :=

### 私有变量 ##################################################################
ifeq ($(strip $(target_type)),lib)
override __target_dir := lib
override __target_file := $(__target_dir)/lib$(project_name).a
else
ifeq ($(strip $(target_type)),dll)
override __target_dir := lib
override __target_file := $(__target_dir)/lib$(project_name).so
else
ifeq ($(strip $(target_type)),bin)
override __target_dir := bin
override __target_file := $(__target_dir)/$(project_name)
endif
endif
endif
override __target_basename := $(notdir $(__target_file))

override __src_files := $(src_files)
ifneq ($(strip $(src_root_dir)),)
override __src_files := $(shell find $(src_root_dir) -type f -name '*.cpp')
endif
ifneq ($(strip $(src_dirs)),)
override __src_files := $(foreach x,$(src_dirs),$(wildcard $(x)/*.cpp))
endif

override __src_files := $(sort $(__src_files))
override __obj_files := $(patsubst %.cpp,%$(__version).o,$(__src_files))
override __dep_files := $(patsubst %.cpp,%.d,$(__src_files))

CXXFLAGS ?= 
CXXFLAGS += $(flags)
ifneq ($(strip $(inc_dirs)),)
CXXFLAGS += $(foreach x,$(inc_dirs),-I$(x))
endif

CPPFLAGS ?=
ifneq ($(strip $(defines)),)
CPPFLAGS += $(foreach x,$(defines),-D$(x))
endif

LDFLAGS ?=
ifneq ($(strip $(lib_dirs)),)
LDFLAGS += $(foreach x,$(lib_dirs),-Wl,-rpath=$(x) -L$(x))
endif

LDLIBS ?=
# LOADLIBES =
ifneq ($(strip $(lib_names)),)
LDLIBS += $(foreach x,$(lib_names),-l$(x))
endif

override __tar := /bin/tar cJf
override __install := /bin/install -m 744 -p
override __mkdir := /bin/mkdir -p
override __testdir := /bin/test -d
override __testfile := /bin/test -f
override __sed := /bin/sed -e
override __mv := /bin/mv

#vpath %.hpp $(inc_dirs)

#############################################################################
.PHONY: all before install clean distclean dist force
## Top
all: before $(__target_file)

%$(__version).o: %.cpp
	$(COMPILE.cc) $(OUTPUT_OPTION) $< $$(wx-config --cxxflags)

%.d: %.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -MM $< | \
$(__sed) 's;^\($(*F)\)\.o *:;$(dir $@)\1$(__version).o:;' > $@

ifeq ($(MAKECMDGOALS),)
-include $(__dep_files)
else
ifeq ($(MAKECMDGOALS),all)
-include $(__dep_files)
endif
endif

before:
	@$(__mkdir) $(__target_dir)

ifeq ($(strip $(target_type)),lib)
$(__target_file): $(__obj_files)
	$(RM) $@
	$(AR) $(ARFLAGS) $@ $^
	$(RANLIB) $@
endif

ifeq ($(strip $(target_type)),dll)
$(__target_file): $(__obj_files)
	$(RM) $@
	$(CXX) $(CXXFLAGS) -shared -o $@ $^
endif

ifeq ($(strip $(target_type)),bin)
$(__target_file): $(__obj_files)
	$(RM) $@
	$(LINK.cc) $^ $(LOADLIBES) $(LDLIBS) -o $@
endif

install:
	$(__testfile) $(__target_file)
	$(__mkdir) $(install_dir)
#	$(__testfile) $(install_dir)/$(__target_basename) && \
$(__mv) $(install_dir)/$(__target_basename) $(install_dir)/$(__target_basename).old
	$(__install) $(__target_file) $(install_dir)

clean:
	$(RM) $(__obj_files) $(__target_file)
	$(RM) $(__dep_files)

distclean: clean
	$(RM) $(dist_file)

dist:
	$(__mkdir) $(dist_dir)
	$(RM) $(dist_file)
	$(__tar) $(dist_file) --exclude-vcs --exclude='$(dist_dir)' \
--exclude='$(__target_dir)' --exclude='*.o' --exclude='*.d' ./
