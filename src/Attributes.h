#ifndef ATTRIBUTES_H
#define ATTRIBUTES_H

#define PACKED __attribute__((packed))
#define ALIGNED(align) __attribute__ ((aligned ((align))))
#define HIDDEN_SYMBOL __attribute__ ((visibility("hidden")))
#define NORETURN [[noreturn]]

#endif //ATTRIBUTES_H
