# How to Commit

> 参考 https://github.com/conventional-changelog/conventional-changelog/tree/master/packages/conventional-changelog-angular
>
> 中文参考（可能过时） https://www.cnblogs.com/wubaiqing/p/10307605.html

## 基本要点

- 使用Signed-off选项（在Clion中勾选）
- 使用除了`Type`以外，可以使用**中文** 
- 使用Clion插件[conventional commits](https://plugins.jetbrains.com/plugin/13389-conventional-commit)格式化commits
- 进行修改时

如果希望手动写commit，参考以下规则

## 基本格式

A commit message consists of a **header**, **body** and **footer**.  The header has a **type**, **scope** and **subject**:

```
<type>(<scope>): <subject>
<BLANK LINE>
<body>
<BLANK LINE>
<footer>
```

The **header** is mandatory and the **scope** of the header is optional.

Any line of the commit message cannot be longer 100 characters! This allows the message to be easier to read on GitHub as well as in various git tools.

### 

### Revert

If the commit reverts a previous commit, it should begin with `revert: `, followed by the header of the reverted commit. In the body it should say: `This reverts commit <hash>.`, where the hash is the SHA of the commit being reverted.

### 

### Type

可用的标签包括`feat`(新特性)，`fix`(修复)，`perf`(性能)，这三项出现在自动生成的changelog中。

另外我们还允许这些标签：`docs`(文档)，`chore`(日常事务)，`stype`(代码风格)，`refactor`(重构)和`test`(测试)。



### Scope

The scope could be anything specifying place of the commit change. For example `$location`, `$browser`, `$compile`, `$rootScope`, `ngHref`, `ngClick`, `ngView`, etc...

### 

### Subject

The subject contains succinct description of the change:

- use the imperative, present tense: "change" not "changed" nor "changes"
- don't capitalize first letter
- no dot (.) at the end

### 

### Body

Just as in the **subject**, use the imperative, present tense: "change" not "changed" nor "changes". The body should include the motivation for the change and contrast this with previous behavior.

### 

### Footer

The footer should contain any information about **Breaking Changes** and is also the place to reference GitHub issues that this commit **Closes**.

**Breaking Changes** should start with the word `BREAKING CHANGE:` with a space or two newlines. The rest of the commit message is then used for this.

A detailed explanation can be found in this [document](https://docs.google.com/document/d/1QrDFcIiPjSLDn3EL15IJygNPiHORgU1_OOAqWjiDU5Y/edit#).

Based on https://github.com/angular/angular.js/blob/master/CONTRIBUTING.md#commit



