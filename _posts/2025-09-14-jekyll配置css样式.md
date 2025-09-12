---
layout: post
title:  jekyll配置css样式
date:   2025-09-14 22:53 +0800
categories: blog update
tag: scss 
---

前几天刚发现我的博客上边有一个黑条，不知道是什么东西，用浏览器看了一下发现好像是组件没覆盖好，然后也觉得主题有点丑，今天找找换主题，主题没换好，在minima的issue看到一个好像是说的我这个问题（其实不是，英文水平有限看错了），看了一下学会了找配置文件，然后就开始改。

前端不是很了解就问ai呗，问了半天一直说就是那么干，然后就是死活改不了样式。一直告诉我要在`/assets/css/style.scss`里改，得一个多小时，问了deepseek问genimi,都是啥也不会，反正最后也不知道怎么问的他就知道了，说要在`/assets/main.scss`里配置，结果就好了。

之前：![before](/assets/images/250914-before.png)

之后：![after](/assets/images/250914-after.png)

最后`/assets/main.scss`是这样的：

```scss
---
---
@import "minima";
// _layout.scss
.site-header {
  border-top: 1px solid $grey-color-light;
  border-bottom: 1px solid $grey-color-light;
  min-height: $spacing-unit * 1.865;

  // Positioning context for the mobile navigation icon
  position: relative;
}
```

这样再运行就可以了，然后我继续找了一下发现这个是在`minima-2.5.1/_includes/head.html`中定义的，在第六行有一句`<link rel="stylesheet" href="{{ "/assets/main.css" | relative_url }}">`
。

至于ai一直说的`/assets/css/style.scss`，我确实也是这么以为的，因为Github上的[教程](https://docs.github.com/en/pages/setting-up-a-github-pages-site-with-jekyll/adding-a-theme-to-your-github-pages-site-using-jekyll#customizing-your-themes-css)就是说在这个位置配置，但是这个是minimal的配置方法，跟我用的主题不一样，原来这个还不是通用的。

之后ai说最佳实践是在根目录建一个`_sass/_style.scss`，然后在`main.scss`中引用他，最后`main.scss`是这样的

```scss
---
---
@import "minima";
@import "style";
```

`_style.scss`是这样的

```scss
// _layout.scss
.site-header {
  border-top: 1px solid $grey-color-light;
  border-bottom: 1px solid $grey-color-light;
  min-height: $spacing-unit * 1.865;

  // Positioning context for the mobile navigation icon
  position: relative;
}
```

只有`main.scss`需要”front matter“，`minima-2.5.1/assets/main.scss`如是说。

> 然后这个Arch上的vscode怎么没法用中文输入法啊，一晚上没发现写文章的时候才发现。我马上就是vim大神了。
