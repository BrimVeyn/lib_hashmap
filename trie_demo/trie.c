/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trie.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvan-pae <bryan.vanpaemel@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 22:31:53 by pollivie          #+#    #+#             */
/*   Updated: 2024/06/23 16:05:32 by bvan-pae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "trie.h"
#include <stdint.h>

#define END_OF_WORD_INDEX 26

void setEndOfWord(TrieNode *self, bool status)
{
	if (self)
		self->children[END_OF_WORD_INDEX] = (void *) status;
}

bool isEndOfWord(TrieNode *self)
{
	return self->children[END_OF_WORD_INDEX] != 0;
}

TrieNode *TrieNodeCreate(void)
{
	TrieNode *self;

	self = (TrieNode *) malloc(sizeof(TrieNode));
	if (!self)
		return (NULL);
	setEndOfWord(self, false);
	memset(self->children, 0x00, ALPHABET_SIZE * sizeof(void *));
	return (self);
}

TrieNode *TrieNodeFindPrefixNode(TrieNode *const self, const char *prefix)
{
	TrieNode *temp;

	temp = self;
	while (*prefix && temp)
	{
		const uint64_t index = (*prefix | 32) - ('a' | 32);
		temp = temp->children[index];
		++prefix;
	}
	return (temp);
}

bool TrieNodeRemoveChild(TrieNode *self, const char *const key, const uint64_t depth)
{
	if (!self)
		return (false);
	if (*key == '\0')
	{
		if (isEndOfWord(self))
		{
			setEndOfWord(self, false);
			if (TrieNodeIsEmpty(self))
			{
				free(self);
				return (true);
			}
			return (false);
		}
	}
	else
	{
		const uint64_t index = (*key| 32) - ('a' | 32);
		if (TrieNodeRemoveChild(self->children[index], key + 1, depth + 1))
		{
			self->children[index] = NULL;
			return (!isEndOfWord(self) && TrieNodeIsEmpty(self));
		}
	}
	return (false);
}

bool TrieNodeIsEmpty(TrieNode *self)
{
	for (uint64_t i = 0; i < ALPHABET_SIZE; ++i)
	{
		if (self->children[i])
			return (false);
	}
	return (true);
}

void TrieNodeDestroy(TrieNode *self)
{
	if (self)
	{
		for (uint64_t i = 0; i < ALPHABET_SIZE; ++i)
		{
			if (self->children[i])
				TrieNodeDestroy(self->children[i]);
		}
		free(self);
	}
}

Trie *TrieCreate(void)
{
	Trie *self;

	self = (Trie *) malloc(sizeof(Trie));
	if (!self)
		return (NULL);
	self->root = TrieNodeCreate();
	if (!self->root)
	{
		free(self);
		return (NULL);
	}
	return (self);
}

void TrieInsert(Trie *const self, const char *const key)
{
	TrieNode   *node;
	const char *ptr;

	ptr = key;
	node = self->root;
	while (*ptr)
	{
		const uint64_t index = (*ptr | 32) - ('a' | 32);
		if (!node->children[index])
			node->children[index] = TrieNodeCreate();
		node = node->children[index];
		++ptr;
	}
	setEndOfWord(node, true);
}

bool TrieSearch(Trie *const self, const char *const key)
{
	TrieNode   *node;
	const char *ptr;

	node = self->root;
	ptr = key;
	while (*ptr)
	{
		const uint64_t index = (*key | 32) - ('a' | 32);
		if (!node->children[index])
			return (false);
		node = node->children[index];
		++ptr;
	}
	return (node != NULL && isEndOfWord(node));
}

bool TrieRemove(Trie *const self, const char *const key)
{
	return (TrieNodeRemoveChild(self->root, key, 0));
}

List *TrieSuggest(Trie *const self, const char *prefix)
{
	List     *suggestions;
	TrieNode *prefix_node;

	suggestions = listCreate();
	prefix_node = TrieNodeFindPrefixNode(self->root, prefix);
	dprintf(2, "%s\n", prefix);
	if (prefix_node)
		TrieCollectSuggestions(prefix_node, strdup(prefix), suggestions);
	return (suggestions);
}

void TrieCollectSuggestions(TrieNode *const node, const char *prefix, List *suggestions)
{
	if (!node)
		return;
	if (isEndOfWord(node))
		listPushBack(suggestions, (uintptr_t) strdup(prefix));
	for (uint64_t i = 0; i < ALPHABET_SIZE; ++i)
	{
		if (node->children[i])
		{
			const uint64_t prefix_len = strlen(prefix);
			const uint64_t new_len = prefix_len + 2;
			char    *const prefix_buffer = (char *) malloc(new_len);
			if (prefix_buffer)
			{
				snprintf(prefix_buffer, new_len, "%s%c", prefix, (uint8_t) ('a' + i));
				TrieCollectSuggestions(node->children[i], prefix_buffer, suggestions);
				free(prefix_buffer);
			}
		}
	}
}

Trie *TrieDestroy(Trie *const self)
{
	if (self)
	{
		TrieNodeDestroy(self->root);
		free(self);
	}
	return (NULL);
}

void TrieCollectWords(TrieNode *node, char *prefix, List *suggestions) {
    if (!node)
        return;
    if (isEndOfWord(node))
        listPushBack(suggestions, (uintptr_t)strdup(prefix));

    char new_prefix[strlen(prefix) + 2];
    for (uint64_t i = 0; i < ALPHABET_SIZE; ++i) {
        if (node->children[i]) {
            snprintf(new_prefix, sizeof(new_prefix), "%s%c", prefix, (uint8_t) ('a' + i));
            TrieCollectWords(node->children[i], new_prefix, suggestions);
        }
    }
}

void TrieBFSForWildcards(TrieNode *node, uint64_t depth, List *nodes) {
    if (depth == 6) {
        listPushBack(nodes, (uintptr_t)node);
        return;
    }

    for (uint64_t i = 0; i < ALPHABET_SIZE; ++i) {
        if (node->children[i]) {
            TrieBFSForWildcards(node->children[i], depth + 1, nodes);
        }
    }
}

List *TrieSearchPattern(Trie *const self, char *pattern) {
    List *nodes, *suggestions;
    TrieNode *node;
    char prefix[7] = {0};

    // Handle the case where the pattern starts with six wildcards
    nodes = listCreate();
    TrieBFSForWildcards(self->root, 0, nodes);

    suggestions = listCreate();
    while (!listIsEmpty(nodes)) {
        node = (TrieNode *)listPopFront(nodes);
        if (node->children['C' - 'A']) {
            snprintf(prefix, sizeof(prefix), pattern);
            TrieCollectWords(node->children['C' - 'A'], prefix, suggestions);
        }
    }

    listDestroy(nodes);
    return suggestions;
}
