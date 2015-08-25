
## Picture
Module that can store scene picture into file.

##### Example:

Stores scene picture each 100 iterations into file with name `pic_$1.png` in directory `pictures`.

```xml
<module name="picture" pattern="pictures/pic_$1.png" iteration="100" />
```

##### Parameters:

| Name        | Type     | Default        | Description |
| ----------- | -------- | -------------- | ----------- |
| `pattern`   | `string` | `image_$1.png` | Path to output file. Pattern can have special `$1` sequence that is replaced by iteration number. Currently is supported only *PNG* files. |
| `iteration` | `int`    | `1`            | Number of iteration to skip before store picture (~ store each Nth iteration). |

