/* Copyright (c) IAIK, Graz University of Technology, 2015.
 * All rights reserved.
 * Contact: http://opensource.iaik.tugraz.at
 * 
 * This file is part of the Secure Block Device Library.
 * 
 * Commercial License Usage
 * Licensees holding valid commercial licenses may use this file in
 * accordance with the commercial license agreement provided with the
 * Software or, alternatively, in accordance with the terms contained in
 * a written agreement between you and SIC. For further information
 * contact us at http://opensource.iaik.tugraz.at.
 * 
 * Alternatively, this file may be used under the terms of the GNU General
 * Public License as published by the Free Software Foundation version 2.
 * 
 * The Secure Block Device Library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with the Secure Block Device Library. If not, see <http://www.gnu.org/licenses/>.
 */
///
/// \file
/// \brief Specifies the Secure Block Device Library's application programmer
/// interface.
///
/// The SBD API is modeled after the POSIX file abstraction.
///

#ifdef __cplusplus
extern "C" {
#endif

#ifndef SECURE_BLOCK_DEVICE_INTERFACE_H_
#define SECURE_BLOCK_DEVICE_INTERFACE_H_

#include "sbdi_config.h"
#include "sbdi_pio.h"
#include "sbdi_crypto.h"
#include "sbdi_cache.h"
#include "sbdi_ctr_128b.h"
#include "sbdi_block.h"
#include "sbdi_hdr.h"
#include "sbdi_crypto_type.h"

#include <sys/types.h>
#include <stdint.h>

/*!
 * \brief converts a Merkle tree error into a secure block device interface
 * error
 *
 * @param mr the Merkle tree error code
 * @return the corresponding secure block device interface error code
 */
static inline sbdi_error_t sbdi_mt_sbdi_err_conv(mt_error_t mr)
{
  switch (mr) {
  case MT_SUCCESS:
    return SBDI_SUCCESS;
  case MT_ERR_OUT_Of_MEMORY:
    return SBDI_ERR_OUT_Of_MEMORY;
  case MT_ERR_ILLEGAL_PARAM:
    return SBDI_ERR_ILLEGAL_PARAM;
  case MT_ERR_ILLEGAL_STATE:
    return SBDI_ERR_ILLEGAL_STATE;
  case MT_ERR_ROOT_MISMATCH:
    return SBDI_ERR_TAG_MISMATCH;
  case MT_ERR_UNSPECIFIED:
    return SBDI_ERR_UNSPECIFIED;
  default:
    return SBDI_ERR_UNSUPPORTED;
  }
}

typedef uint8_t sbdi_sym_mst_key_t[32];

/*!
 * \brief enumeration type for defining secure block device lseek constants
 */
typedef enum sbdi_whence {
  SBDI_SEEK_SET = 1, //!< SBDI_SEEK_SET
  SBDI_SEEK_CUR = 2, //!< SBDI_SEEK_CUR
  SBDI_SEEK_END = 3 //!< SBDI_SEEK_END
} sbdi_whence_t;

struct secure_block_device_interface {
  sbdi_pio_t *pio;
  sbdi_crypto_t *crypto;
  void *mt;
  sbdi_hdr_v1_t *hdr;
  sbdi_bc_t *cache;
  sbdi_bl_data_t write_store_dat[2];
  sbdi_block_t write_store[2];
  size_t offset;
};

sbdi_t *sbdi_create(sbdi_pio_t *pioypto);
void sbdi_delete(sbdi_t *sbdi);

sbdi_error_t sbdi_open(sbdi_t **s, sbdi_pio_t *pio, sbdi_crypto_type_t ct, sbdi_sym_mst_key_t mkey,
    mt_hash_t root);
sbdi_error_t sbdi_close(sbdi_t *sbdi, sbdi_sym_mst_key_t mkey, mt_hash_t root);

sbdi_error_t sbdi_pread(ssize_t *rd, sbdi_t *sbdi, void *buf, size_t nbyte,
    off_t offset);
sbdi_error_t sbdi_pwrite(ssize_t *wr, sbdi_t *sbdi, const void *buf,
    size_t nbyte, off_t offset);

sbdi_error_t sbdi_read(ssize_t *rd, sbdi_t *sbdi, void *buf, size_t nbyte);
sbdi_error_t sbdi_write(ssize_t *wr, sbdi_t *sbdi, const void *buf,
    size_t nbyte);

sbdi_error_t sbdi_lseek(off_t *new_off, sbdi_t *sbdi, off_t offset,
    sbdi_whence_t whence);

sbdi_error_t sbdi_fsync(sbdi_t *sbdi, sbdi_sym_mst_key_t mkey);
sbdi_error_t sbdi_sync(sbdi_t *sbdi, sbdi_sym_mst_key_t mkey, mt_hash_t root);

#endif /* SECURE_BLOCK_DEVICE_INTERFACE_H_ */

#ifdef __cplusplus
}
#endif
