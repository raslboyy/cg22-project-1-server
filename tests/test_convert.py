import aiohttp

P6 = "P6 2 2 255 aaabbbcccddd"


def to_form_data(file: str):
    with aiohttp.MultipartWriter('form-data') as data:
        payload = aiohttp.payload.StringPayload(file)
        payload.set_content_disposition('form-data', name='file')
        data.append_payload(payload)

    headers = {
        'Content-Type': 'multipart/form-data; boundary=' + data.boundary,
    }

    return data, headers


async def test_basic(service_client):
    data, headers = to_form_data(P6)
    response = await service_client.get('/convert?from=RGB&to=HSL&channel=all', data=data, headers=headers)
    assert response.status == 200
