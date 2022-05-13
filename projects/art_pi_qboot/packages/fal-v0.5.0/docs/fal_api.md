# FAL API

## Find flash devices

````C
const struct fal_flash_dev *fal_flash_device_find(const char *name)
````

| Parameters | Description |
| :----- | :----------------------- |
| name | Flash device name |
| return | If the search is successful, the flash device object will be returned, and if the search fails, it will return NULL |

## Find flash partition
```C
const struct fal_partition *fal_partition_find(const char *name)
```
| Parameters | Description |
| :----- | :----------------------- |
| name | Flash partition name |
| return | If the search is successful, the flash partition object will be returned, and if the search fails, it will return NULL |

## Get the partition table

````C
const struct fal_partition *fal_get_partition_table(size_t *len)
````

| Parameters | Description |
| :----- | :----------------------- |
| len | Length of partition table |
| return | partition table |

## Temporarily set the partition table

The default partition table is automatically mounted when FAL is initialized. Using this setting will temporarily modify the partition table and will **lost** this setting after restarting

````C
void fal_set_partition_table_temp(struct fal_partition *table, size_t len)
````

| Parameters | Description |
| :----- | :----------------------- |
| table | partition table |
| len | Length of partition table |

## read data from partition
````C
int fal_partition_read(const struct fal_partition *part, uint32_t addr, uint8_t *buf, size_t size)
````

| Parameters | Description |
| :----- | :----------------------- |
| part | Partition object |
| addr | Offset address relative to the partition |
| buf | The buffer to store the data to be read |
| size | The size of the data to be read |
| return | Returns the actual data size read |

## write data to the partition

````C
int fal_partition_write(const struct fal_partition *part, uint32_t addr, const uint8_t *buf, size_t size)
````

| Parameters | Description |
| :----- | :----------------------- |
| part | Partition object |
| addr | Offset address relative to the partition |
| buf | The buffer to store the data to be written |
| size | The size of the data to be written |
| return | Returns the actual data size written |

## Erase partition data

````C
int fal_partition_erase(const struct fal_partition *part, uint32_t addr, size_t size)
````

| Parameters | Description |
| :----- | :----------------------- |
| part | Partition object |
| addr | Offset address relative to the partition |
| size | The size of the erased area |
| return | Returns the size of the area actually erased |

## Erase entire partition data```C
int fal_partition_erase_all(const struct fal_partition *part)
````

| Parameters | Description |
| :----- | :----------------------- |
| part | Partition object |
| return | Returns the size of the area actually erased |

## print partition table

````c
void fal_show_part_table(void)
````

## create block device

This function can create a corresponding block device according to the specified partition name, so as to mount the file system on the specified partition

````C
struct rt_device *fal_blk_device_create(const char *parition_name)
````

| Parameters | Description |
| :----- | :----------------------- |
| partition_name | partition name |
| return | If the creation is successful, it will return the corresponding block device, if it fails, it will return empty |

## Create MTD Nor Flash device

This function can create a corresponding MTD Nor Flash device according to the specified partition name, so as to mount the file system on the specified partition
```C
struct rt_device *fal_mtd_nor_device_create(const char *parition_name)
```
| Parameters | Description |
| :------------ | :------------------------------------------------ ------------------ |
| partition_name | partition name |
| return | If the creation is successful, it will return the corresponding MTD Nor Flash device, if it fails, it will return empty |

## create character device

This function can create a corresponding character device according to the specified partition name, so that the partition can be operated through the deivice interface or the devfs interface. After POSIX is enabled, the partition can also be operated through the oepn/read/write functions.

````C
struct rt_device *fal_char_device_create(const char *parition_name)
````

| Parameters | Description |
| :------------ | :------------------------------------------------ ------- |
| partition_name | partition name |
| return | If the creation is successful, it will return the corresponding character device, if it fails, it will return empty |
