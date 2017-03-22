#include "test.h"
#include <tsdemux.h>
#include <stdio.h>

void test_video_stream(void);
void test_audio_stream(void);
void test_hierarchy(void);
void test_registration(void);
void test_data_stream_alignment(void);
void test_target_background_grid(void);
void test_video_window(void);
void test_conditional_access(void);
void test_iso_639_language(void);

int main(int argc, char **argv)
{
    test_video_stream();
    test_audio_stream();
    test_hierarchy();
    test_registration();
    test_data_stream_alignment();
    test_target_background_grid();
    test_video_window();
    test_conditional_access();
    test_iso_639_language();
    return 0;
}

void test_video_stream(void)
{
    test_start("video_stream_descriptor");

    TSDDescriptorVideoStream desc;
    const uint8_t data[] = {
        0x02, // tag
        0x03, // length
        0b10101011, // mulit frame rate flag (1), frame_rate_code(4), MPEG_1_only_flag (1), constrained_param (1), still pic(1)
        0x66, // profile level indicator
        0b01011111, // chroma(2), frame rate ext. flag(1), reserved(5)
    };

    TSDCode res;
    res = tsd_parse_descriptor_video_stream(NULL, sizeof(data), &desc);
    test_assert_equal(TSD_INVALID_DATA, res, "invalid data");
    res = tsd_parse_descriptor_video_stream(data, 2, &desc);
    test_assert_equal(TSD_INVALID_DATA_SIZE, res, "invalid data size");
    res = tsd_parse_descriptor_video_stream(data, sizeof(data), NULL);
    test_assert_equal(TSD_INVALID_ARGUMENT, res, "invalid argument");

    res = tsd_parse_descriptor_video_stream(data, sizeof(data), &desc);
    test_assert_equal(TSD_OK, res, "TSD_OK");
    test_assert_equal(0x02, desc.tag, "tag");
    test_assert_equal(0x03, desc.length, "length");
    test_assert_equal(TSD_DFVS_MULTI_FRAME_RATE, desc.flags & TSD_DFVS_MULTI_FRAME_RATE, "multi frame rate flag");
    test_assert_equal(0, desc.flags & TSD_DFVS_MPEG_1_ONLY, "mpeg 1 only flag");
    test_assert_equal(TSD_DFVS_CONSTRAINED_PARAM, desc.flags & TSD_DFVS_CONSTRAINED_PARAM, "constrained flag");
    test_assert_equal(TSD_DFVS_STILL_PIC, desc.flags & TSD_DFVS_STILL_PIC, "still picture flag");
    test_assert_equal(0x05, desc.frame_rate_code, "frame rate code");

    test_end();
}

void test_audio_stream(void)
{
    test_start("audio_stream_descriptor");

    TSDDescriptorAudioStream desc;
    const uint8_t data[] = {
        0x03, // tag
        0x01, // length
        0b10100111, // free format flag(1), ID(1), layer(2), variable rate audio indicator(1), reserved(3)
    };

    TSDCode res;
    res = tsd_parse_descriptor_audio_stream(NULL, sizeof(data), &desc);
    test_assert_equal(TSD_INVALID_DATA, res, "invalid data");
    res = tsd_parse_descriptor_audio_stream(data, 2, &desc);
    test_assert_equal(TSD_INVALID_DATA_SIZE, res, "invalid data size");
    res = tsd_parse_descriptor_audio_stream(data, sizeof(data), NULL);
    test_assert_equal(TSD_INVALID_ARGUMENT, res, "invalid argument");

    res = tsd_parse_descriptor_audio_stream(data, sizeof(data), &desc);
    test_assert_equal(TSD_OK, res, "TSD_OK");
    test_assert_equal(0x03, desc.tag, "tag");
    test_assert_equal(0x01, desc.length, "length");
    test_assert_equal(desc.flags & TSD_DFAS_FREE_FORMAT, TSD_DFAS_FREE_FORMAT, "free format");
    test_assert_equal(desc.flags & TSD_DFAS_ID, 0, "ID");
    test_assert_equal(desc.flags & TSD_DFAS_VAR_RATE_AUDIO_IND, 0, "variable rate audio indicator");
    test_assert_equal(desc.layer, 0x02, "layer");
    test_end();
}

void test_hierarchy(void)
{
    test_start("hierarchy_descriptor");

    TSDDescriptorHierarchy desc;
    const uint8_t data[] = {
        0x04, // tag
        0x04, // length
        0xF3, // reserved(4), type(4)
        0xD9, // resvered(2), layer index(6)
        0xE8, // reserved(2), embedded layer index(6)
        0xF7, // reserved(2), channel(6)
    };

    TSDCode res;
    res = tsd_parse_descriptor_hierarchy(NULL, sizeof(data), &desc);
    test_assert_equal(TSD_INVALID_DATA, res, "invalid data");
    res = tsd_parse_descriptor_hierarchy(data, 2, &desc);
    test_assert_equal(TSD_INVALID_DATA_SIZE, res, "invalid data size");
    res = tsd_parse_descriptor_hierarchy(data, sizeof(data), NULL);
    test_assert_equal(TSD_INVALID_ARGUMENT, res, "invalid argument");

    res = tsd_parse_descriptor_hierarchy(data, sizeof(data), &desc);
    test_assert_equal(TSD_OK, res, "TSD_OK");
    test_assert_equal(desc.tag, 0x04, "tag");
    test_assert_equal(desc.length, 0x04, "length");
    test_assert_equal(desc.type, 0x03, "type");
    test_assert_equal(desc.layer_index, 0x19, "layer index");
    test_assert_equal(desc.embedded_layer_index, 0x28, "embedded layer index");
    test_assert_equal(desc.channel, 0x37, "channel");
    test_end();
}

void test_registration(void)
{
    test_start("registration_descriptor");

    TSDDescriptorRegistration desc;
    const uint8_t data[] = {
        0x05, // tag
        0x07, // length
        0xAD, 0x54, 0xF5, 0x1C, // format idenitifier (4)
        0xFC, 0xAF, 0xED // additional identifier info (3)
    };

    TSDCode res;
    res = tsd_parse_descriptor_registration(NULL, sizeof(data), &desc);
    test_assert_equal(TSD_INVALID_DATA, res, "invalid data");
    res = tsd_parse_descriptor_registration(data, 2, &desc);
    test_assert_equal(TSD_INVALID_DATA_SIZE, res, "invalid data size");
    res = tsd_parse_descriptor_registration(data, sizeof(data), NULL);
    test_assert_equal(TSD_INVALID_ARGUMENT, res, "invalid argument");

    res = tsd_parse_descriptor_registration(data, sizeof(data), &desc);
    test_assert_equal(TSD_OK, res, "TSD_OK");
    test_assert_equal(desc.tag, 0x05, "tag");
    test_assert_equal(desc.length, 0x07, "length");
    test_assert_equal(desc.format_identifier, 0xAD54F51C, "format identifier");
    test_assert_equal_ptr((size_t)desc.additional_id_info, (size_t)&data[6], "additional identifier info");
    test_assert_equal(desc.additional_id_info_length, 3, "additional identifier info length");
    test_end();
}

void test_data_stream_alignment(void)
{
    test_start("data_stream_descriptor");

    TSDDescriptorDataStreamAlignment desc;
    const uint8_t data[] = {
        0x06, // tag
        0x01, // length
        0x02, // type
    };

    TSDCode res;
    res = tsd_parse_descriptor_data_stream_alignment(NULL, sizeof(data), &desc);
    test_assert_equal(TSD_INVALID_DATA, res, "invalid data");
    res = tsd_parse_descriptor_data_stream_alignment(data, 2, &desc);
    test_assert_equal(TSD_INVALID_DATA_SIZE, res, "invalid data size");
    res = tsd_parse_descriptor_data_stream_alignment(data, sizeof(data), NULL);
    test_assert_equal(TSD_INVALID_ARGUMENT, res, "invalid argument");

    res = tsd_parse_descriptor_data_stream_alignment(data, sizeof(data), &desc);
    test_assert_equal(TSD_OK, res, "TSD_OK");
    test_assert_equal(desc.tag, 0x06, "tag");
    test_assert_equal(desc.length, 0x01, "length");
    test_assert_equal(desc.type, 0x02, "type");
    test_end();
}

void test_target_background_grid(void)
{
    test_start("target_background_grid_descriptor");

    TSDDescriptorTargetBackgroundGrid desc;
    const uint8_t data[] = {
        0x07, // tag
        0x04, // length
        0xFE, 0xDC, 0xBA, 0x98, // horizontal size(14), vertical size(14), aspect ratio(4)
    };

    TSDCode res;
    res = tsd_parse_descriptor_target_background_grid(NULL, sizeof(data), &desc);
    test_assert_equal(TSD_INVALID_DATA, res, "invalid data");
    res = tsd_parse_descriptor_target_background_grid(data, 2, &desc);
    test_assert_equal(TSD_INVALID_DATA_SIZE, res, "invalid data size");
    res = tsd_parse_descriptor_target_background_grid(data, sizeof(data), NULL);
    test_assert_equal(TSD_INVALID_ARGUMENT, res, "invalid argument");

    res = tsd_parse_descriptor_target_background_grid(data, sizeof(data), &desc);
    test_assert_equal(TSD_OK, res, "TSD_OK");
    test_assert_equal(desc.tag, 0x07, "tag");
    test_assert_equal(desc.length, 0x04, "length");
    test_assert_equal(desc.horizontal_size, 0x3FB7, "horizontal size");
    test_assert_equal(desc.vertical_size, 0x0BA9, "vertical size");
    test_assert_equal(desc.aspect_ratio_info, 0x08, "aspect ratio info");
    test_end();
}

void test_video_window(void)
{
    test_start("video_window_descriptor");

    TSDDescriptorVideoWindow desc;
    const uint8_t data[] = {
        0x08, // tag
        0x04, // length
        0xAB, 0x09, 0xF3, 0x62, // horizontal offset(14), vertical offset(14), windows priority(4)
    };

    TSDCode res;
    res = tsd_parse_descriptor_video_window(NULL, sizeof(data), &desc);
    test_assert_equal(TSD_INVALID_DATA, res, "invalid data");
    res = tsd_parse_descriptor_video_window(data, 2, &desc);
    test_assert_equal(TSD_INVALID_DATA_SIZE, res, "invalid data size");
    res = tsd_parse_descriptor_video_window(data, sizeof(data), NULL);
    test_assert_equal(TSD_INVALID_ARGUMENT, res, "invalid argument");

    res = tsd_parse_descriptor_video_window(data, sizeof(data), &desc);
    test_assert_equal(TSD_OK, res, "TSD_OK");
    test_assert_equal(desc.tag, 0x08, "tag");
    test_assert_equal(desc.length, 0x04, "length");
    test_assert_equal(desc.horizontal_offset, 0x2AC2, "horizontal offset");
    test_assert_equal(desc.vertical_offset, 0x1F36, "vertical offset");
    test_assert_equal(desc.window_priority, 0x02, "window priority");
    test_end();
}

void test_conditional_access(void)
{
    test_start("conditional_access_descriptor");

    TSDDescriptorConditionalAccess desc;
    const uint8_t data[] = {
        0x09, // tag
        0x07, // length
        0x99, 0x88, // CA System ID
        0xEF, 0xD7, // reserved(3), CA_PID (13)
        0xAB, 0xCD, 0xEF, // private data bytes
    };

    TSDCode res;
    res = tsd_parse_descriptor_conditional_access(NULL, sizeof(data), &desc);
    test_assert_equal(TSD_INVALID_DATA, res, "invalid data");
    res = tsd_parse_descriptor_conditional_access(data, 2, &desc);
    test_assert_equal(TSD_INVALID_DATA_SIZE, res, "invalid data size");
    res = tsd_parse_descriptor_conditional_access(data, sizeof(data), NULL);
    test_assert_equal(TSD_INVALID_ARGUMENT, res, "invalid argument");

    res = tsd_parse_descriptor_conditional_access(data, sizeof(data), &desc);
    test_assert_equal(TSD_OK, res, "TSD_OK");
    test_assert_equal(desc.tag, 0x09, "tag");
    test_assert_equal(desc.length, 0x07, "length");
    test_assert_equal(desc.ca_system_id, 0x9988, "CA system ID");
    test_assert_equal(desc.ca_pid, 0x0FD7, "CA PID");
    test_assert_equal(desc.private_data_bytes_length, 3, "private data bytes length");
    test_assert_equal_ptr((size_t)desc.private_data_bytes, (size_t)&data[6], "private data bytes");
    test_end();
}

void test_iso_639_language(void)
{
    test_start("iso_639_language_descriptor");

    TSDDescriptorISO639Language desc;
    const uint8_t data[] = {
        0x0A, // tag
        0x08, // length
        0x02, 0x03, 0x04, // iso 639 language code
        0x01, // audio type
        0xB7, 0x2F, 0x6A, // iso 639 language code
        0x03, // audio type
    };

    TSDCode res;
    res = tsd_parse_descriptor_iso639_language(NULL, sizeof(data), &desc);
    test_assert_equal(TSD_INVALID_DATA, res, "invalid data");
    res = tsd_parse_descriptor_iso639_language(data, 2, &desc);
    test_assert_equal(TSD_INVALID_DATA_SIZE, res, "invalid data size");
    res = tsd_parse_descriptor_iso639_language(data, sizeof(data), NULL);
    test_assert_equal(TSD_INVALID_ARGUMENT, res, "invalid argument");

    res = tsd_parse_descriptor_iso639_language(data, sizeof(data), &desc);
    test_assert_equal(TSD_OK, res, "TSD_OK");
    test_assert_equal(desc.tag, 0x0A, "tag");
    test_assert_equal(desc.length, 0x08, "length");
    test_assert_equal(desc.language_length, 2, "language length");
    test_assert_equal(desc.iso_language_code[0], 0x020304, "iso 639 language 0");
    test_assert_equal(desc.audio_type[0], 0x01, "audio type 0");
    test_assert_equal(desc.iso_language_code[1], 0xB72F6A, "iso 639 language 1");
    test_assert_equal(desc.audio_type[1], 0x03, "audio type 1");
    test_end();
}